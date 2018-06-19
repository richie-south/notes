const printer = require('node-thermal-printer')
const {json} = require('micro')

printer.init({
  type: 'epson',
  interface: '/dev/usb/lp0',
  width: 40,
})

printer.isPrinterConnected((isConnected) => {
  printer.execute((err) => {})
})

printer.setTextNormal()
printer.bold(true)

module.exports = async (req, res) => {
  const {tasks} = await json(req)

  tasks.map(printTask)
  res.end('Printing tasks :D')
}

function printTask(taskString) {
  let hasPrintedFirstLine = false
  const chars = taskString.split('')

  const print = (str) => {
    if (hasPrintedFirstLine) {
      printer.println(`      ${str}`)
    } else {
      printer.println(`(  ): ${str}`)
    }
  }

  chars.reduce((line, char, i) => {
    if (i === chars.length - 1) {
      print(line + char)
      hasPrintedFirstLine = true
    }
    if (line.length >= 26) {
      print(line)
      hasPrintedFirstLine = true
      return char
    }
    return line + char
  })
}
