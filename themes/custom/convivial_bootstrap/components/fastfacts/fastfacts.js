/**
 * @file
 * JS for Fast Facts counting effect.
 */

(function (Drupal) {
  Drupal.behaviors.fastfacts = {
    attach: function (context, settings) {
      const fastFacts = context.querySelectorAll('.fastfact__value')
      let fastFactsItem = []

      if (!fastFacts) {
        return
      }

      // Construct fast fact list item
      fastFacts.forEach(function (item, index) {
        const endNumber = parseInt(item.innerHTML, 10)
        fastFactsItem.push(item)
        fastFactsItem[index]['trigger'] = false
        fastFactsItem[index]['startNumber'] = 0
        fastFactsItem[index]['endNumber'] = endNumber
        item.innerHTML = '0'
      })

      context.addEventListener('scroll', function () {
        fastFactsItem.forEach(function (item) {
          if (isScrolledIntoView(item) && item['trigger'] !== true) {
            item['trigger'] = true
            animateValue(item, item['startNumber'], item['endNumber'], 3000)
          }
        })
      })

      // Utility
      function isScrolledIntoView (element) {
        const rect = element.getBoundingClientRect()
        const elemTop = rect.top
        const elemBottom = rect.bottom
        return (elemTop >= 0) && (elemBottom <= window.innerHeight)
      }

      function animateValue (item, start, end, duration) {
        if (start === end) {
          return
        }
        const range = end - start
        let current = start
        const increment = end > start ? 1 : -1
        const stepTime = Math.abs(Math.floor(duration / range))
        const timer = setInterval(function () {
          current += increment
          item.innerHTML = current
          if (current === end) {
            clearInterval(timer)
          }
        }, stepTime)
      }
    },
  }
})(Drupal)
