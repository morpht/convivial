#!/usr/bin/env bash

while [ "$1" != "" ]
do
  case "$1" in
    -c | --commit)
      shift
      if [ "$1" != "" ] && [ "$( echo $1 | cut -c 1)" != "-" ]; then
        commit_hash="$1"
        shift
      fi;;
    -s | --source)
      shift
      if [ "$1" != "" ] && [ "$( echo $1 | cut -c 1)" != "-" ]; then
        source_dir="$1"
        shift
      fi;;
#        -d|--destination)
#         destination_dir=$2
#         shift
#         shift;;
    -h | --help)
      echo "This command copies commits from source site to Convivial profile."
      echo "Following arguments are mandatory:"
      echo "  -c|--commit <hash> Commit hash."
      echo "  -s|--source <path> Source directory."
      echo "Following arguments are optional:"
      echo "  -n|--no-commit Do not commit, just apply patch."
      echo "  -g|--continue After fixing all failed hunks, continue to clean up config and commit."
      echo "To get this message, type 'profile -h|--help'"
      exit;;
    -n | --no-commit)
      no_commit=1
      shift;;
    -g | --continue)
      continue=1
      shift;;
    *)
      echo "Only -c, -s, -d, -h, -n, -g flags are allowed. Type profile -h to get help."; exit
  esac
done

if [ "$(git status -s)" != "" ] && [ -z "$continue" ]; then
  echo "There are changed files in the project. Cannot continue"; exit
fi

if [ -z "$continue" ]; then
  if [ -z "$commit_hash" ]; then
    echo "No commit is defined. Type profile -h to get help."; exit
  fi

  if [ -z "$source_dir" ]; then
    echo "No source directory is defined. Type profile -h to get help."; exit
  fi

#  if [ -z $destination_dir ]; then
#    echo "No destination directory is defined"; exit
#  fi

  current_dir=$(pwd)

  cd "$source_dir"

  patch_file=$(git format-patch -1 "$commit_hash")

  mkdir -p "$current_dir"/patches
  mv "$patch_file" "$current_dir"/patches

  cd "$current_dir"

  sed -i 's/web\/themes/themes/g' patches/"$patch_file"
  sed -i 's/web\/modules/modules/g' patches/"$patch_file"
  sed -i 's/config\/default/config\/install/g' patches/"$patch_file"

  patch -p1 -f < patches/"$patch_file"
fi

# If some of the hunks failed, patch will exit. Or not.
if [ "$(find . -name *.rej)" != "" ]; then
  echo "There are some rejected hunks. Inspect them, remove resolved .rej files and continue with profile -g."; exit
fi

if [ "$continue" ]; then
  if [ "$(ls -1q patches | wc -l)" -gt 1 ]; then
    echo "Multiple patch files. Cannot decide which one to use. Exiting."; exit
  elif [ "$(ls -1q patches | wc -l)" -lt 1 ]; then
    echo "No patch files. Exiting."; exit
  else
    patch_file="$(ls -1q patches)"
  fi
fi

find . -name "*.orig" | xargs -r rm

grep -l '^uuid:' config/install/* | xargs -r sed -i '/^uuid:/d'
# Not really perfect, but default config hash is used only with _core
grep -l '^_core:$' config/install/* | xargs -r sed -i -e '/^_core:$/d' -e '/default_config_hash/d'
if [ $no_commit ]; then
  exit
fi

author=$(sed '2!d' patches/"$patch_file" | cut -c 7-)
message=$(sed '4!d' patches/"$patch_file" | cut -c 18-)

rm patches/"$patch_file"

git add .
git commit -m "$message" --author="$author"
