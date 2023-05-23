#!/usr/bin/env bash

# fail on first error, fail on any error in a pipeline:
set -e
set -o pipefail

project_name=$(grep 'name:.*' /app/.lando.yml | cut -f2- -d: | xargs)
remote="live"
aliasplatform="pantheon.$project_name"
site="default"


PARAMS=""
while (("$#")); do
  case "$1" in
  -h | --help)
    echo -e "Usage: lando fresh-install\t[-h | --help] [-l | --from-local] [-r | --remote \"environment\"]"
    echo -e  ""
    echo -e  "\t-h, --help"
    echo -e  "\t\tPrints out this help text."
    echo -e  ""
    echo -e  "\t-l, --from-local"
    echo -e  "\t\tUse local copies of database dumps when available."
    echo -e  "\t\tFresh database will be fetched as default when this argument is not used."
    echo -e  "\t\tFresh database will be fetched if there is no local copy available."
    echo -e  ""
    echo -e  "\t-nc, --no-cim"
    echo -e  "\t\tSkip config import at the end."
    echo -e  ""
    echo -e  "\t-r, --remote"
    echo -e  "\t\tSelects the source environemt for databse and files. Defaults to $remote."
    exit 0
    ;;
  -l | --from-local)
    from_local=true
    shift
    ;;
  -nc | --no-cim)
    no_cim=true
    shift
    ;;
  -r | --remote)
    if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
      remote="$2"
      shift 2
    else
      echo "Error: Argument for $1 is missing" >&2
      exit 1
    fi
    ;;
  -* | --*=) # unsupported flags
    echo "Error: Unsupported flag $1. Use --help to print information about using this script." >&2
    exit 1
    ;;
  *) # preserve positional arguments
    PARAMS="$PARAMS $1"
    shift
    ;;
  esac
done
# set positional arguments in their proper place
eval set -- "$PARAMS"

## Start filling up local sites.
composer install

echo "# Processing: $site:"

# This is mainly to discover SSH key issues early.
echo "Make sure $remote site is ready: drush @$aliasplatform.$remote status"
drush @$aliasplatform.$remote status

# This is mainly to check that local DB credentials line up and the site is ready for DB.
echo "Make sure local site is ready: drush @$aliasplatform.lando status"
drush @$aliasplatform.lando status || echo "WARNING: Local site is NOT ready, but let's continue anyway..."

echo "Make sure we can connect to the local DB via drush, otherwise there is no point to continue:"
echo "SHOW STATUS LIKE 'Uptime';" | drush @$aliasplatform.lando sqlc > /dev/null
echo "   successfully connected to the local DB."

echo "* Fresh copy of settings.local.php"
if [ ! -f "/app/web/sites/$site/settings/settings.local.php" ]; then
  if [ -d "/app/web/sites/$site/settings" ]; then
    cp -TRv /app/.lando/templates/settings.local.php /app/web/sites/$site/settings/settings.local.php
  fi
fi

echo "* Make sure files directory is ready."
mkdir -p /app/web/sites/$site/files/private
mkdir -p /app/web/sites/$site/files/tmp
chmod -R 777 /app/web/sites/$site/files
# And prepare local directory for database dumps.
mkdir -p /app/.local_dbs

# Check if local source is preferred.
if [ "$from_local" = true ]; then
  echo "* Existing local DB is preferred. Looking for /app/.local_dbs/$remote.sql.gz..."
  if [ -f "/app/.local_dbs/$remote.sql.gz" ]; then
    echo "* Found /app/.local_dbs/$remote.sql.gz. Importing..."
  else
    echo "* Local copy at /app/.local_dbs/$remote.sql.gz. not found. Trying to get one from $remote..."
    echo "* drush -Dssh.tty=0 @$aliasplatform.$remote sql-dump --gzip --structure-tables-key=common | pv > /app/.local_dbs/$remote.sql.gz"
    drush -Dssh.tty=0 @$aliasplatform.$remote sql-dump --gzip --structure-tables-key=common | pv > /app/.local_dbs/$remote.sql.gz
  fi
else
  echo "* Fresh DB dump from $remote is preferred. Saving it to /app/.local_dbs/$remote.sql.gz for later use."
  echo "* drush -Dssh.tty=0 @$aliasplatform.$remote sql-dump --gzip --structure-tables-key=common | pv > /app/.local_dbs/$remote.sql.gz"
  drush -Dssh.tty=0 @$aliasplatform.$remote sql-dump --gzip --structure-tables-key=common | pv > /app/.local_dbs/$remote.sql.gz
fi

echo "Wipe local DB: drush @$aliasplatform.lando sql-drop -y"
drush @$aliasplatform.lando sql-drop -y

dump_size=$(ls -l /app/.local_dbs/$remote.sql.gz | awk '{ print $5 }')
# gzip runs after pv to get the same pv statistic / size output as when copying gziped db from remote above.
echo "Import DB: cat /app/.local_dbs/$remote.sql.gz | pv -s $dump_size | gzip -d | drush -Dssh.tty=0 @$aliasplatform.lando sqlc"
cat /app/.local_dbs/$remote.sql.gz | pv -s $dump_size | gzip -d | drush -Dssh.tty=0 @$aliasplatform.lando sqlc

echo "Cache rebuild: drush @$aliasplatform.lando cr"
drush @$aliasplatform.lando cr

if [ "$no_cim" = true ]; then
  echo "* Skipping config import."
else
  echo "* Config import."
  echo "* drush -Dssh.tty=0 @$aliasplatform.lando config-import --yes"
  drush -Dssh.tty=0 @$aliasplatform.lando config-import --yes
fi

echo -e "\n\n"

# Show login links together at the end.
echo "The site is ready. You can now log in:"
drush @$aliasplatform.lando uli

echo -e "\n\n"
echo "Run following command if you want to download files too:"
echo "lando drush rsync @$aliasplatform.$remote:%files @$aliasplatform.lando:%files --verbose"

exit 0
