
let numberOfSamples = 0
let activeSample = 1

let button
  
const triggerSample = () => {
  Bela.data.sendBuffer(0, 'i', [activeSample, 1]);
  Bela.data.sendBuffer(0, 'i', [activeSample, 0]);
}
  
function setup() {
  // listen for data events
  Bela.data.target.addEventListener('buffer-ready', event => {
    if(event.detail == 0 && typeof Bela.data.buffers[0] != 'undefined') {
      numberOfSamples = Bela.data.buffers[0][0]
    }
  })

  //Create a canvas of dimensions given by current browser window
  createCanvas(windowWidth, windowHeight)

  textFont('Courier New')

  button = createButton("trigger")
  button.position(0, 0)
  button.mousePressed(triggerSample)
}

function draw() {
  
}
