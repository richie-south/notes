const {json} = require('micro')
const fetch = require('isomorphic-fetch')
const _async = require("async")

/*str
  .split('[')
  .filter(s => !s.includes('x]'))
  .map(s => s.split(']')[1])
  .filter(Boolean)
  .map(s => s
      .replace('\n', '')
      .replace('å', 'a')
      .replace('ä', 'a')
      .replace('ö', 'o')
      .trim())*/

/* _async.eachSeries([
  'task 1'
], (task, callback) => {
  console.log('doing request!')
  fetch(`http://192.168.1.237/print?task=${encodeURI(task)}`)
    .then(() => {
      console.log('done', task)
      setTimeout(() => {
        callback()
      }, 300)

    })
    .catch(() => {
      console.log('error', task)
      callback()
    })
}) */

module.exports = async (req, res) => {
  //const {tasks} = await json(req)


  res.end()
}

