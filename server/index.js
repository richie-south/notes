const {json} = require('micro')
const fetch = require('isomorphic-fetch')
const _async = require("async")
const CronJob = require('cron').CronJob

// real '0 0 5 * * *'
/* new CronJob('* * * * * *', function() {
  console.log('hello')

}, null, true, 'Europe/Stockholm')
 */
function printWeather() {
  _async.waterfall([
    function (callback) {
      printTitle('Weather')
        .then(() => callback(null, 'done')) 
    },
    function (_, callback) {
      printLine('hello')
      .then(() => callback(null, 'done'))
    },
    function (_, callback) {
      printLine('bajs')
        .then(() => callback(null, 'done'))
    }
  ], () => {
    console.log('kingen')
  })
}

function printTitle (title) {
  return fetch(`http://192.168.1.237/print/title?title=${title}`)
    .catch((error) => {
      console.log('error', error)
    })
}

function printLine (line) {
  return fetch(`http://192.168.1.237/print/line?text=${line}`)
    .catch((error) => {
      console.log('error', error)
    })
}

printWeather()

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
/*
module.exports = async (req, res) => {
  //const {tasks} = await json(req)


  res.end()
}

 */
