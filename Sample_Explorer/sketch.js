const sketch = p => {

  const controls = new Controls()
  

  let sampleName = ''
  let activeSample
  let numberOfSamples = 0
  let sampleButtons = []
  let scrubber = null
  
  p.setup = () => {
    controls.selectSample(0)
    
    p.createCanvas(p.windowWidth, p.windowHeight)
    p.rectMode(p.CORNER)

    p.textSize(32)
    p.textAlign(p.LEFT, p.TOP)

  }

  p.draw = () => {
    const newSampleName = controls.readSampleName()
    if (sampleName !== newSampleName && newSampleName !== undefined) {
      sampleName = newSampleName
      p.stroke(255, 255, 255)
      p.rect(0, 0, 500, 70)
      p.text(sampleName.join(''), 60, 0)
    }

    const info = controls.readInfo()
    const newActiveSample = info[0]
    const activeSampleLength = info[1]
    const newNumberOfSamples = info[2]

    if (activeSample !== newActiveSample) {
      activeSample = newActiveSample

      if (scrubber !== null) {
        scrubber.remove()
      }
      
      scrubber = p.createSlider(0, activeSampleLength, 0)
      scrubber.position(0, 100)
      scrubber.input(controls.scrubSample(activeSample, scrubber))
    }
    
    if (numberOfSamples !== newNumberOfSamples) {
      numberOfSamples = newNumberOfSamples
      
      for (b of sampleButtons) {
        b.remove()
      }

      for (let i = 0; i < numberOfSamples; i++) {
        let button = p.createButton(`${i}`)
        button.position(30 * i, 60)
        button.mousePressed(() => controls.selectSample(i))
        sampleButtons.push(button)
      }
    }
  }
}

class Controls {
  constructor() {
    this.channels = {
      output: {
        select: 0,
        scrub: 1,
      },
      input: {
        info: 0,
        name: 1,
      },
    }
  }

  selectSample(n) {
    Bela.data.sendBuffer(this.channels.output.select, 'int', [n+1])
  }
  
  readSampleName() {
    return Bela.data.buffers[this.channels.input.name]
  }

  readInfo() {
    return Bela.data.buffers[this.channels.input.info]
  }

  scrubSample(n, scrubber) {
    const f = () => {
      Bela.data.sendBuffer(this.channels.output.scrub, 'int', [n+1, scrubber.value()])
    }
    return f
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
