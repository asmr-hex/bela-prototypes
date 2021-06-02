const sketch = p => {

  const controls = new Controls()
  let triggerButton
  let switchSampleButton
  let sampleName = ''
  
  p.setup = () => {
    p.createCanvas(p.windowWidth, p.windowHeight)

    triggerButton = p.createButton("trigger")
    triggerButton.position(0, 0)
    triggerButton.mousePressed(() => controls.trigger())
  }

  p.draw = () => {
    const newSampleName = controls.readSampleName()
    if (sampleName !== newSampleName && newSampleName !== undefined) {
      sampleName = newSampleName
      p.textSize(32)
      p.text(sampleName.join(''), 100, 80)
    } 

  }
}

class Controls {
  constructor() {
    this.channels = {
      output: {
        trigger: 0,
      },
      input: {
        name: 1,
      },
    }
  }

  readSampleName() {
    return Bela.data.buffers[this.channels.input.name]
  }
  
  trigger() {
    console.log("DOINK")
    Bela.data.sendBuffer(this.channels.output.trigger, 'int', [1])
  }
}

new p5(sketch)

// let numberOfSamples = 0
// let activeSample = 1

// let button
  
// const triggerSample = () => {
//   Bela.data.sendBuffer(0, 'i', [activeSample, 1]);
//   Bela.data.sendBuffer(0, 'i', [activeSample, 0]);
// }

// Bela.control.addEventListener('new-connection', e => {
//   console.log("YOOOOO")
//   console.log(e)
// })

// function setup() {

//   // listen for data events
//   Bela.data.target.addEventListener('buffer-ready', event => {
//     console.log("HAHA")
//     if(event.detail == 0 && typeof Bela.data.buffers[0] != 'undefined') {
//       numberOfSamples = Bela.data.buffers[0][0]
//       console.log(numberOfSamples)
//     }
//   })

//   //Create a canvas of dimensions given by current browser window
//   createCanvas(windowWidth, windowHeight)

//   textFont('Courier New')

//   button = createButton("trigger")
//   button.position(0, 0)
//   button.mousePressed(triggerSample)
// }

// function draw() {

// }
