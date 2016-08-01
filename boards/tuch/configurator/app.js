///////////////////////////////////////////////////////////////////////
// MIDI

var in_notes = [0,1,2,3,4,5,6,7];           // パトランプから来るNote Number
var touch_notes_min = 20;
var touch_notes_max = 25;                    // mi:muz:touchから来るNote Number
var out_notes = [43,45,46,47,38,36,40,41];  // 缶へ出力するNote Number

// Can 1 : 43 (CaraMints) can3.png
//       : 45 (bell) can4.png
// Can 2 : 46 (無印缶) can1.png
//       : 47 (みかん) can2.png
// Can 3 : 38 (Twining-side) can5.png
//       : 36 (Twining-bottom) can8.png
// Can 4 : 40 (cheen) can6.png
//       : 41 (Cookie) can7.png

var midi = new poormidi();

var mes = {};
var u8arr = new Uint8Array(4);

var isPatLampPlay = 0;

var readTimer = null;

var notenums = [null,null,null,null,null,null];

function checkNoteNums(num){
  for(var cnt = 0;cnt < 6;cnt ++){
    if(notenums[cnt] == num){
      return[cnt];
    }
  }
  return(-1);
}

midi.onDeviceChange = function(e){
  if(readTimer != null){
    clearTimeout(readTimer);
  }
  readTimer = setTimeout(readalldata(),2000);
}

midi.onMidiEvent = function(e){
  console.log("["+e.data[0]+"]["+e.data[1]+"]["+e.data[2]+"]");
  var message = e.data[0] & 0xf0;
  if(e.data[0] === 0xbF){
    if(e.data[1] === 119){
      mode = 1;
    }else if((e.data[1] >= 110)&&(e.data[1] < 116)){
      if(mode == 1){
        mode = 0;
        var sensor_num = e.data[1]-110;
        $("#i_ch"+sensor_num).val(e.data[2]);
        notenums[sensor_num] = e.data[2];
      }
    }else{
      mode = 0;
    }
  }else if(message == 0x90){
    var ch = checkNoteNums(e.data[1]);
    if(ch != -1){
      startLed(ch);
    }
  }
}

function delay(millisec){
  var start = new Date();
  while((new Date()-start)<millisec);
}


// Read
function readalldata(){
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,110,0); // Sensor#0
  delay(100);
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,111,0); // Sensor#1
  delay(100);
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,112,0); // Sensor#2
  delay(100);
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,113,0); // Sensor#3
  delay(100);
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,114,0); // Sensor#4
  delay(100);
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,115,0); // Sensor#5
  delay(100);
  midi.sendCtlChange(15,119,2);
  midi.sendCtlChange(15,116,0); // I2C Slave Addr
  delay(100);

}

for(cnt = 0;cnt < 6;cnt ++){
  $("#i_ch"+cnt).change(function() {
    var value = $(this).val();
    var id = $(this).attr("id");
    var ch = id.replace( /i_ch/g ,"");
    var chno = 110 + Number(ch);
    midi.sendCtlChange(15,119,1);
    midi.sendCtlChange(15,chno,parseInt(value));
  });
}


///////////////////////////////////////////////////////////////////////
// Pixi.js
// 
// Front Layer : title.spr      (Title) 
//       Layer : cansContainer  (缶) 
//       Layer : faceContainer  (顔) 
//       Layer : duckContainer  (あひる) 
// back  Layer : linesContainer (background Lines)
// 
// Animation
var stage = new PIXI.Container();
var width = window.innerWidth;
var height = window.innerHeight;
var renderer = PIXI.autoDetectRenderer(width, height, {autoResize: true});
renderer.backgroundColor = 0xc92a7a;
renderer.antialias = true;
document.getElementById("pixiview").appendChild(renderer.view);

///////////////////////////////////////////////////////////////////////
// Background Lines 

var linesContainer = new PIXI.Container();
var colorMatrix =  [0.5,1,0,0,0,0.5,0,1,0,0,0.8,0.4,0,0,0,0.3]; // default
stage.addChild(linesContainer);

var colors = [
  0xFF0080,
  0xFFBA00,
  0x1CDB1C,
  0xFF3A00,
  0x4768E6,
  0x00B3FF,
  0xAE0BBF
];

var line = {};
line.grp = new PIXI.Graphics();
linesContainer.addChild(line.grp);

function initLine(){
  // direction: 1. left->right 2. right->left 3. top->bottom 4. bottom->top
  line.colornum = Math.floor(Math.random() * colors.length);
  line.width = Math.floor(30 + (Math.random() * 170));
  line.opacity = (Math.random() + 3) / 4;
  line.grp.lineStyle(line.width,colors[line.colornum],line.opacity);
  line.direction = Math.floor(Math.random() * 4);

//  console.log("initLine(colornum:"+line.colornum+", linewidth:"+ line.width +", direction:"+line.direction+")");

  if(line.direction == 1){
   line.x = -50;
   line.y = Math.random() * height;
  }else if(line.direction == 2){
   line.x = width + 50;
   line.y = Math.random() * height;
  }else if(line.direction == 3){
   line.x = Math.random() * width;
   line.y = -50;
  }else{
   line.x = Math.random() * width;
   line.y = height + 50;
  }
  line.grp.moveTo(line.x,line.y);
}

function updateLine(){
  var isRenew = 0;
  line.grp.moveTo(line.x,line.y);
  if(line.direction == 1){
    line.x += Math.random() * 100;
    line.y += (Math.random() - 0.5) * 200;
    if(line.x > width){
      isRenew = 1;
    }
  }else if(line.direction == 2){
    line.x -= Math.random() * 100;
    line.y += (Math.random() - 0.5) * 200;
    if(line.x < 0){
      isRenew = 1;
    }
  }else if(line.direction == 3){
    line.x += (Math.random() - 0.5) * 200;
    line.y += Math.random() * 100;
    if(line.y > width){
      isRenew = 1;
    }
  }else{
    line.x += (Math.random() - 0.5) * 200;
    line.y -= Math.random() * 100;
    if(line.y < 0){
      isRenew = 1;
    }
  }
  line.grp.lineTo(line.x, line.y);
  if(isRenew == 1){
    initLine();
    isRenew = 0;
  }
}

initLine();


///////////////////////////////////////////////////////////////////////
// あひる

var duckContainer = new PIXI.Container();
stage.addChild(duckContainer);

var ducknames = [
  ['./img/duck1-front-small.png','./img/duck1-left-small.png','./img/duck1-right-small.png'],
  ['./img/duck2-front-small.png','./img/duck2-left-small.png','./img/duck1-right-small.png'],
  ['./img/duck3-front-small.png','./img/duck3-left-small.png','./img/duck1-right-small.png'],
  ['./img/duck4-front-small.png','./img/duck4-left-small.png','./img/duck1-right-small.png'],
  ['./img/duck5-front-small.png','./img/duck5-left-small.png','./img/duck1-right-small.png']
];

function addDuck(){
  // direction: 1. left->right 2. right->left 3. top->bottom 4. bottom->top

  var direction = Math.floor(Math.random() * 4);
  var num = Math.floor(Math.random() * ducknames.length);
  var texrec = 0;
  if(direction == 2){
    texrec = 1;
  }else if(direction == 1){
    texrec = 2;
  }
  var ftex = PIXI.Texture.fromImage(ducknames[num][texrec]);
  var spr = new PIXI.Sprite(ftex);
  spr.anchor.x = 0.5;
  spr.anchor.y = 0.5;
//  spr.scale.x = 0.3;
//  spr.scale.y = 0.3;
  spr.direction = direction;

  if(direction == 1){
   spr.position.x = -50;
   spr.position.y = Math.random() * height;
  }else if(direction == 2){
   spr.position.x = width + 50;
   spr.position.y = Math.random() * height;
  }else if(direction == 3){
   spr.position.x = Math.random() * width;
   spr.position.y = -50;
  }else{
   spr.position.x = Math.random() * width;
   spr.position.y = height + 50;
  }
  spr.speed = (Math.random()*20) + 5;
  duckContainer.addChild(spr);
}

function updateDucks(){
  for(var cnt=0;cnt < duckContainer.children.length; cnt ++){
    var child = duckContainer.children[cnt];
    var isDelete = 0;
    child.rotation += 0.1;

    if(child.direction == 1){
      child.position.x += child.speed;
      child.position.y += (Math.random() - 0.5) * 5;
      if(child.position.x > (width+100)){
        isDelete = 1;
      }
    }else if(child.direction == 2){
      child.position.x -= child.speed;
      child.position.y += (Math.random() - 0.5) * 5;
      if(child.position.x < -100){
        isDelete = 1;
      }
    }else if(child.direction == 3){
      child.position.x += (Math.random() - 0.5) * 5;
      child.position.y += child.speed;
      if(line.y > (width+100)){
        isDelete = 1;
      }
    }else{
      child.position.x += (Math.random() - 0.5) * 5;
      child.position.y -= child.speed;
      if(child.position.y < -100){
        isDelete = 1;
      }
    }
    if(isDelete == 1){
//      initLine();
      duckContainer.removeChild(child);
    }
  }
}


///////////////////////////////////////////////////////////////////////
// Title
var titleContainer = new PIXI.Container();
stage.addChild(titleContainer);


////////
// device 
var remtex = PIXI.Texture.fromImage("./img/device.png");
var rem = new PIXI.Sprite(remtex);

var DEVICE_IMG_WIDTH = 2004;
var DEVICE_IMG_HEIGHT = 1295;

function getDeviceSize(){
  var sw = width / DEVICE_IMG_WIDTH;
  var sh = height / DEVICE_IMG_HEIGHT;

  if(sw > sh){
    return (sh * 0.9);
  }else{
    return (sw * 0.9);
  }
}

function initDevice(){
  rem.position.x = width/2;
  rem.position.y = height/2;
  rem.anchor.x = 0.5;
  rem.anchor.y = 0.5;
  rem.scale.x = 0.4;
  rem.scale.y = 0.4;

//  rem.scale.x = getDeviceSize();
//  rem.scale.y = rem.scale.x;
  titleContainer.addChild(rem);
}

initDevice();

//////////////
// Led

var ledContainer = new PIXI.Container();
stage.addChild(ledContainer);
//var isLedStart = [false,false,false,false,false,false];
var ledCount = [0,0,0,0,0,0];
var LED_COUNT_MAX = 8;

var remtex = PIXI.Texture.fromImage("./img/led.png");

var ledPositions = [
  {x:-172,y:-70},
  {x:-172,y:82},
  {x:-56,y:82},
  {x:58,y:82},
  {x:172,y:82},
  {x:172,y:-70}
];

var ledsObj = new Array();

function initLeds(){
  for(var cnt = 0;cnt < 6;cnt ++){
    var led = new PIXI.Sprite(remtex);
    led.position.x = (width/2)+ledPositions[cnt].x;
    led.position.y = (height/2)+ledPositions[cnt].y;
    led.anchor.x = 0.5;
    led.anchor.y = 0.5;
    led.scale.x = 0.4;
    led.scale.y = 0.4;
    ledContainer.addChild(led);
    ledsObj.push(led);
  }
}

function refreshLeds(){
  for(var cnt = 0;cnt < 6;cnt ++){
    ledsObj[cnt].position.x = (width/2)+ledPositions[cnt].x;
    ledsObj[cnt].position.y = (height/2)+ledPositions[cnt].y;

  }
}

function startLed(num){
  ledCount[num] = LED_COUNT_MAX;
}

function updateLeds(){
  for(var cnt = 0;cnt < 6;cnt ++){
    if(ledCount[cnt] != 0){
      ledsObj[cnt].scale.x += 0.1;
      ledsObj[cnt].scale.y += 0.1;
      ledCount[cnt] --;
      if(ledCount[cnt] == 0){
        ledsObj[cnt].scale.x = 0.4;
        ledsObj[cnt].scale.y = 0.4;
      }
    }
  }
}

initLeds();

///////////////////////////////////////////////////////////////////////
// Animation Frame

requestAnimationFrame(animate);

var sprites_num = 0;
var frames = 0;
var fcount = 0;

function animate(){
  requestAnimationFrame(animate); // 次の描画タイミングでanimateを呼び出す

  if((frames % 2) == 1){
    updateLeds();
  }

  if((frames % 8) == 3){
    updateLine();
  }
  if((frames % 8) == 5){
    fcount ++;
    fcount %= 8;
  }
  if((frames % 8 == 7)){
    updateDucks();
  }

  if((frames % 30) == 1){
    if(Math.random() > 0.5){
      addDuck();
    }
  }

  if((frames % 2) == 0){
    renderer.render(stage);   // 描画する
  }
  frames ++;
  frames %= 60;
}



///////////////////////////////////////////////////////////////////////
// resizeing
var resizeTimer = false;
$(window).resize(function() {
    if (resizeTimer !== false) {
        clearTimeout(resizeTimer);
    }
    resizeTimer = setTimeout(function() {
      width = window.innerWidth;
      height = window.innerHeight;

      initDevice();
      refreshLeds();

      renderer.resize(width, height);


    }, 200);
});


