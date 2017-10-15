
var keyboardInfo = {
    downKeys:{},
    pressedKeys:{},
    pressedKeyHandlers:{},
    downKeyHandlers:{},
    pressedKeysHandlers:[],
    downKeysHandlers:[],
    pressedKeysListHandlers:[],
    downKeysListHandlers:[],
    handleKeyDown: function (keyCode) {
        if (!this.downKeys[keyCode]) {
            this.pressedKeys[keyCode] = true;
        }
    
        this.downKeys[keyCode] = true;
    },
    handleKeyUp: function (keyCode) {
        this.downKeys[keyCode] = false;
        this.pressedKeys[keyCode] = false;
    },
    resetNewlyPressedKeys: function () {
        this.pressedKeys = {};
        
    },
    addEventHandlerKeyPressed: function (keyCode, handler) {
        if (this.pressedKeyHandlers[keyCode]) {
            this.pressedKeyHandlers[keyCode].push(handler);
        } else {
            this.pressedKeyHandlers[keyCode] = [handler];
        }
    },
    addEventHandlerKeyDown: function (keyCode, handler) {
        if (this.downKeyHandlers[keyCode]) {
            this.downKeyHandlers[keyCode].push(handler);
        } else {
            this.downKeyHandlers[keyCode] = [handler];
        }
    },
    addEventHandlerKeysPressed: function (handler) {
        this.pressedKeysHandlers.push(handler);
    },
    addEventHandlerKeysDown: function (handler) {
        this.downKeysHandlers.push(handler);
    },
    addEventHandlerKeysPressedList: function (handler) {
        this.pressedKeysHandlers.push(handler);
    },
    addEventHandlerKeysDownList: function (handler) {
        this.downKeysHandlers.push(handler);
    },
    callback: function () {
        for (var key in this.pressedKeys) {
            if (this.pressedKeys[key] && this.pressedKeyHandlers[key]) {
                for (var idx in this.pressedKeyHandlers[key]) {
                    this.pressedKeyHandlers[key][idx]();
                }
            }
        }
        
        for (var idx in this.pressedKeysHandlers) {
            for (var key in this.pressedKeys) {
                if (this.pressedKeys[key]) {
                    this.pressedKeysHandlers[idx](key);
                }
            }
        }
        
        for (var idx in this.pressedKeysListHandlers) {
            this.pressedKeysListHandlers[idx](this.pressedKeys);
        }
        
        this.resetNewlyPressedKeys();

        for (var key in this.downKeys) {
            if (this.downKeys[key] && this.downKeyHandlers[key]) {
                for (var idx in this.downKeyHandlers[key]) {
                    this.downKeyHandlers[key][idx]();
                }
            }
        }
        
        for (var idx in this.downKeysHandlers) {
            for (var key in this.downKeys) {
                if (this.downKeys[key]) {
                    this.downKeysHandlers[idx](key);
                }
            }
        }
        
        for (var idx in this.downKeysListHandlers) {
            this.downKeysListHandlers[idx](this.downKeys);
        }
    },
    initHandlers: (function () {
        document.onkeydown = function (event) { keyboardInfo.handleKeyDown(String.fromCharCode(event.keyCode)); };
        document.onkeyup = function (event) { keyboardInfo.handleKeyUp(String.fromCharCode(event.keyCode)); };
    })()
};

//keyboardInfo.addEventHandlerKeyDown('F', function () { console.log ("F has been pressed"); });

var mouseInfo = {
    leftClickHandler: [],
    leftDownHandler: [],
    leftReleaseHandler: [],
    rightClickHandler: [],
    rightDownHandler: [],
    rightReleaseHandler: [],
    middleClickHandler: [],
    middleDownHandler: [],
    middleReleaseHandler: [],
    left: {
        isClick: false,
        isDown: false,
        isRelease: false,
        clickPos: {x:-1, y:-1}
    },
    right: {
        isClick: false,
        isDown: false,
        isRelease: false,
        clickPos: {x:-1, y:-1}
    },
    middle: {
        isClick: false,
        isDown: false,
        isRelease: false,
        clickPos: {x:-1, y:-1}
    },
    mouse: {
        pos: {x:-1, y:-1},
        deltaPos: {x:0, y:0}
    },
    reset: function () {
        this.mouse.deltaPos = {x:0, y:0};
        this.left.isClick = false;
        this.left.isRelease = false;
        this.right.isClick = false;
        this.right.isRelease = false;
        this.middle.isClick = false;
        this.middle.isRelease = false;
    },
    mouseDown: function (buttons, pos) {
        if (buttons == 1) { //left click
            this.left.isDown = true;
            this.left.isClick = true;
            this.left.clickPos.x = pos.x, this.left.clickPos.y = pos.y;
        } else if (buttons == 2) { // right click
            this.right.isDown = true;
            this.right.isClick = true;
            this.right.clickPos.x = pos.x, this.right.clickPos.y = pos.y;
        } else if (buttons == 4) { //middle click
            this.middle.isDown = true;
            this.middle.isClick = true;
            this.middle.clickPos.x = pos.x, this.middle.clickPos.y = pos.y;
        }
    },
    mouseUp: function (buttons) {
        if (buttons < 4) {
            if (this.middle.isDown) {
                this.middle.isRelease = true;
            }
            
            this.middle.isDown = false;
            this.middle.isClick = false;
        } else {
            buttons -= 4;
        }
        
        if (buttons < 2) {
            if (this.right.isDown) {
                this.right.isRelease = true;
            }
            
            this.right.isDown = false;
            this.right.isClick = false;
        } else {
            buttons -= 2;
        }
        
        if (buttons < 1) {
            if (this.left.isDown) {
                this.left.isRelease = true;
            }
            
            this.left.isDown = false;
            this.left.isClick = false;
        } else {
            buttons -= 1;    
        }
    },
    mouseMove: function (pos) {
        if (this.mouse.pos.x !== -1) {
            this.mouse.deltaPos.x += pos.x - this.mouse.pos.x;
        }
        if (this.mouse.pos.y !== -1) {
            this.mouse.deltaPos.y += pos.y - this.mouse.pos.y;
        }
        
        this.mouse.pos = {x:pos.x, y:pos.y};    
    },
    initHandlers: (function () {
        var mouseDown = function (event) {
            mouseInfo.mouseDown(event.buttons, {x: event.pageX, y: event.pageY});

            event.preventDefault();
            return false;
        };

        var mouseUp = function (event) {
            mouseInfo.mouseUp(event.buttons);

            event.preventDefault();
            return false;
        };

        var mouseMove = function (event) {
            mouseInfo.mouseMove({x: event.pageX, y: event.pageY});

            event.preventDefault();
            return false;
        };

        document.onmousedown = mouseDown;
        document.onmouseup = mouseUp;
        document.onmouseout = mouseUp;
        document.onmousemove = mouseMove;
    }) (),
    addLeftClickHandler: function (handler) {
        this.leftClickHandler.push(handler);
    },
    addRightClickHandler: function (handler) {
        this.rightClickHandler.push(handler);
    },
    addMiddleClickHandler: function (handler) {
        this.middleClickHandler.push(handler);
    },
    addLeftDownHandler: function (handler) {
        this.leftDownHandler.push(handler);
    },
    addRightDownHandler: function (handler) {
        this.rightDownHandler.push(handler);
    },
    addMiddleDownHandler: function (handler) {
        this.middleDownHandler.push(handler);
    },    
    addLeftReleaseHandler: function (handler) {
        this.leftReleaseHandler.push(handler);
    },
    addRightReleaseHandler: function (handler) {
        this.rightReleaseHandler.push(handler);
    },
    addMiddleReleaseHandler: function (handler) {
        this.middleReleaseHandler.push(handler);
    },  
    callback: function (handler) {
        if (this.left.isClick) {
            for (var idx in this.leftClickHandler) {
                this.leftClickHandler[idx](this.left.clickPos);
            }
        }
        if (this.right.isClick) {
            for (var idx in this.rightClickHandler) {
                this.rightClickHandler[idx](this.right.clickPos);
            }
        }    
        if (this.middle.isClick) {
            for (var idx in this.middleClickHandler) {
                this.middleClickHandler[idx](this.middle.clickPos);
            }
        }    
    
        if (this.left.isDown) {
            for (var idx in this.leftDownHandler) {
                this.leftDownHandler[idx](this.left.clickPos, this.mouse.pos, this.mouse.deltaPos);
            }
        }
        if (this.right.isDown) {
            for (var idx in this.rightDownHandler) {
                this.rightDownHandler[idx](this.left.clickPos, this.mouse.pos, this.mouse.deltaPos);
            }
        }    
        if (this.middle.isDown) {
            for (var idx in this.middleDownHandler) {
                this.middleDownHandler[idx](this.left.clickPos, this.mouse.pos, this.mouse.deltaPos);
            }
        }
        
        if (this.left.isRelease) {
            for (var idx in this.leftReleaseHandler) {
                this.leftReleaseHandler[idx](this.left.clickPos);
            }
        }
        if (this.right.isRelease) {
            for (var idx in this.rightReleaseHandler) {
                this.rightReleaseHandler[idx](this.right.clickPos);
            }
        }    
        if (this.middle.isRelease) {
            for (var idx in this.middleReleaseHandler) {
                this.middleReleaseHandler[idx](this.middle.clickPos);
            }
        } 
        
        this.reset();
    }
};

// mouseInfo.addLeftClickHandler(function (clickPos) { console.log ("Click at (" + clickPos.x + "," + clickPos.y + ")"); });

function loadTextFile(loadableResource) {
    var request = new XMLHttpRequest();
    request.open("GET", loadableResource.resourceFileName);
    console.log ('Load "' + loadableResource.resourceFileName + '"');
    
    request.onload = function () {
        console.log ('Loaded "' + loadableResource.resourceFileName + '"!');
        loadableResource.resourceLoadHandler (request.responseText);
    }
    request.onerror = function () {
        console.log ('Error loading "' + loadableResource.resourceFileName + '"!');
    }
    request.onabort = function () {
        console.log ('Aborted load of "' + loadableResource.resourceFileName + '"!');
    }
    request.onprogress = function (oEvent) {
        console.log ('Load "' + loadableResource.resourceFileName + '" progress ... ' + (oEvent.loaded * 100/oEvent.total) + '%');
    }
    request.send();
}

function loadImageFile(loadableResource) {
    var image = new Image();
    console.log ('Load "' + loadableResource.resourceFileName + '"');
    
    image.onload = function () {
        console.log ('Loaded "' + loadableResource.resourceFileName + '"!');
        loadableResource.resourceLoadHandler (image);
    }
    image.onerror = function () {
        console.log ('Error loading "' + loadableResource.resourceFileName + '"!');
    }
    image.onabort = function () {
        console.log ('Aborted load of "' + loadableResource.resourceFileName + '"!');
    }
    image.onprogress = function (oEvent) {
        console.log ('Load "' + loadableResource.resourceFileName + '" progress ... ' + (oEvent.loaded * 100/oEvent.total) + '%');
    }
    
    image.src = loadableResource.resourceFileName;
}

var bufferObj = (function () {
    var buffer = function (bufferType, bufferData) {
        this.bufferId = gl.createBuffer();
        this.bufferType = bufferType;
        
        this.dataLength = bufferData.length;
        
        gl.bindBuffer(this.bufferType, this.bufferId);
        
        if (bufferType === gl.ARRAY_BUFFER) {
            gl.bufferData(this.bufferType, new Float32Array(bufferData), gl.STATIC_DRAW);
        } else if (bufferType === gl.ELEMENT_ARRAY_BUFFER) {
            gl.bufferData(this.bufferType, new Uint16Array(bufferData), gl.STATIC_DRAW);
        }
        
        // just a guess
        this.dataSize = 3;
        this.dataOffset = 0;
        this.dataStride = 0;
    }
    
    buffer.prototype.bindBuffer = function () {
        gl.bindBuffer(this.bufferType, this.bufferId);
    }
    
    buffer.prototype.unbindBuffer = function () {
        gl.bindBuffer(this.bufferType, 0);
    }
    
    buffer.prototype.getBufferId = function () {
        return this.bufferId;
    }
    
    buffer.prototype.setSizeStrideOffset = function (size, offset, stride) {
        this.dataSize = size;
        this.dataOffset = offset;
        this.dataStride = stride;
        
        return this;
    }
    
    buffer.prototype.getNumItems = function () {
        return this.dataLength / (this.dataSize + this.dataStride);
    }
       
    return buffer;
}) ();

function getShaderSrc(id) {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
        return null;
    }

    var str = "";
    var child = shaderScript.firstChild;
    while (child) {
        if (child.nodeType == 3) {
            str += child.textContent;
        }
        child = child.nextSibling;
    }

    return str;
}

var shaderObj = (function () {
    var shaderProgObj = function (shaderType, shaderSrc) {
        this.shaderId = gl.createShader(shaderType);
        gl.shaderSource(this.shaderId, shaderSrc);
        gl.compileShader(this.shaderId);
        
        if (!gl.getShaderParameter(this.shaderId, gl.COMPILE_STATUS)) {
            console.log (gl.getShaderInfoLog(this.shaderId));
        }
    }
    
    shaderProgObj.prototype.getShaderId = function () {
        return this.shaderId;
    }
        
    return shaderProgObj;
}) ();

var shaderProg = (function () {
    var shaderProgObj = function () {
        this.programId = gl.createProgram();
    }
    
    shaderProgObj.prototype.addShader = function (shader) {
        gl.attachShader(this.programId, shader.getShaderId ());
        
        return this;
    }
    
    shaderProgObj.prototype.finalise = function () {
        gl.linkProgram(this.programId);
        
        if (!gl.getProgramParameter(this.programId, gl.LINK_STATUS)) {
            console.log("Could not finish linking the shaders");
        }
        
        return this;
    }
    
    shaderProgObj.prototype.getProgId = function () {
        return this.programId;
    }
    
    shaderProgObj.prototype.getUniformLoc = function (uniformName) {
        return gl.getUniformLocation (this.getProgId(), uniformName);
    }
    
    shaderProgObj.prototype.vertexAttribPointer = function (buffer, attrib) {
        buffer.bindBuffer();
        var attribLoc = gl.getAttribLocation (this.getProgId (), attrib);
        
        if (attribLoc == -1) {
            return;
        }
        
        gl.vertexAttribPointer(attribLoc, buffer.dataSize, gl.FLOAT, false, buffer.dataOffset, buffer.dataStride);
        gl.enableVertexAttribArray(attribLoc);
    }
    
    shaderProgObj.prototype.use = function () {
        gl.useProgram(this.programId);
    }
    
    shaderProgObj.prototype.setUniformMatrix4fv = function (matrix4, uniformName) {
        var matLoc = this.getUniformLoc (uniformName);
        gl.uniformMatrix4fv(matLoc, false, matrix4.getData());
    }
    
    shaderProgObj.prototype.setUniformVector3f = function (vector3, uniformName) {
        var vecLoc = this.getUniformLoc (uniformName);
        gl.uniform3f(vecLoc, vector3.data[0], vector3.data[1], vector3.data[2]);
    }
    
    shaderProgObj.prototype.setUniformBool = function (boolValue, uniformName) {
        var boolLoc = this.getUniformLoc (uniformName);
        gl.uniform1i(boolLoc, boolValue); 
    }

    shaderProgObj.prototype.setUniformFloat = function (floatValue, uniformName) {
        var floatLoc = this.getUniformLoc (uniformName);
        gl.uniform1f(floatLoc, floatValue);
    }
    
    shaderProgObj.prototype.setUniformInt = function (intValue, uniformName) {
        var intLoc = this.getUniformLoc (uniformName);
        gl.uniform1i(intLoc, intValue);
    }
    
    shaderProgObj.prototype.setTexture2D = function (texture, uniformName, texLoc) {
        gl.activeTexture(gl.TEXTURE0 + texLoc);
        gl.bindTexture(gl.TEXTURE_2D, texture.texture);
        texLoc = gl.getUniformLocation(this.getProgId (), uniformName)
        gl.uniform1i(texLoc, texLoc);
    }
    
    return shaderProgObj;
}) ();

function constructShader(vertexShaderName, fragmentShaderName) {
    returnObj = new shaderProg();
    
    if (vertexShaderName) {
        returnObj.addShader(new shaderObj(gl.VERTEX_SHADER, getShaderSrc(vertexShaderName)));
    }
    if (fragmentShaderName) {
        returnObj.addShader(new shaderObj(gl.FRAGMENT_SHADER, getShaderSrc(fragmentShaderName)));
    }
    
    returnObj.finalise();
    return returnObj;
}

var renderableTex2D = (function () {
    function textObj () {
        
    }
    
    
    return textObj;
}) ();

var tex2D = (function () {   
    function texture2DObj (imageName) {
        this.resourceFileName = imageName;
        loadImageFile (this);
    }
    
    texture2DObj.prototype.resourceLoadHandler = function (imageVar) {
        this.texture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, this.texture);
        gl.pixelStorei(gl.UNPACK_COLORSPACE_CONVERSION_WEBGL, true);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, imageVar);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.generateMipmap(gl.TEXTURE_2D);
        gl.bindTexture(gl.TEXTURE_2D, null);
    }
    
    texture2DObj.prototype.getTexture = function () {
        return this.texture;
    }
    
    return texture2DObj;
}) ();

var vec3 = (function () {
    function vecObj (vec) {
        this.data = vec;
    }
    
    vecObj.prototype.set = function (vec) {
        this.data = vec;
        return this;
    }
    
    vecObj.prototype.getData = function () {
        return this.data;
    }
    
    return vecObj;
}) ();

var mat4 = (function () {
    function setIdentity (object, matDim) {   
        for (var horIdx = 0; horIdx < matDim; horIdx++) {
            for (var verIdx = 0; verIdx < matDim; verIdx++) {
                var value = 0;
                
                if (horIdx === verIdx) {
                    value = 1;
                }
                
                object.data[horIdx * matDim + verIdx] = value;
            }
        }
    }
    
    var matObj = function () {
        this.data = [];
        setIdentity(this, 4);
    }
    
    matObj.prototype.getData = function () {
        return this.data;
    }
    
    matObj.prototype.reset = function () {
        setIdentity(this, 4);
        
        return this;
    };
    
    matObj.prototype.clear = function () {
        for (var horIdx = 0; horIdx < 16; horIdx++) {
            this.data[horIdx] = 0;
        }
    }
    
    matObj.prototype.rotateX = function (angle) {
        var c = Math.cos(angle);
        var s = Math.sin(angle);
        var mv1 = this.data[1], mv5 = this.data[5], mv9 = this.data[9];
            
        this.data[1] = this.data[1]*c-this.data[2]*s;
        this.data[5] = this.data[5]*c-this.data[6]*s;
        this.data[9] = this.data[9]*c-this.data[10]*s;

        this.data[2] = this.data[2]*c+mv1*s;
        this.data[6] = this.data[6]*c+mv5*s;
        this.data[10] = this.data[10]*c+mv9*s;
        
        return this;
    };
    
    matObj.prototype.rotateY = function (angle) {
        var c = Math.cos(angle);
        var s = Math.sin(angle);
        var mv0 = this.data[0], mv4 = this.data[4], mv8 = this.data[8];
            
        this.data[0] = c*this.data[0]+s*this.data[2];
        this.data[4] = c*this.data[4]+s*this.data[6];
        this.data[8] = c*this.data[8]+s*this.data[10];

        this.data[2] = c*this.data[2]-s*mv0;
        this.data[6] = c*this.data[6]-s*mv4;
        this.data[10] = c*this.data[10]-s*mv8;
        
        return this;
    };
    
    matObj.prototype.rotateZ = function (angle) {
        var c = Math.cos(angle);
        var s = Math.sin(angle);
        var mv0 = this.data[0], mv4 = this.data[4], mv8 = this.data[8];

        this.data[0] = c * this.data[0] - s * this.data[1];
        this.data[4] = c * this.data[4] - s * this.data[5];
        this.data[8] = c * this.data[8] - s * this.data[9];

        this.data[1] = c * this.data[1] + s * mv0;
        this.data[5] = c * this.data[5] + s * mv4;
        this.data[9] = c * this.data[9] + s * mv8;
        
        return this;
    };
    
    matObj.prototype.translateXYZ = function (vectorXYZ) {
        this.data[12] += vectorXYZ.data[0];
        this.data[13] += vectorXYZ.data[1];
        this.data[14] += vectorXYZ.data[2];
        
        return this;
    }

    matObj.prototype.scale = function (value) {
        return this.scaleXYZ({data:[value, value, value]});        
    }
    
    matObj.prototype.scaleXYZ = function (vectorXYZ) {
        this.data[0] *= vectorXYZ.data[0];
        this.data[5] *= vectorXYZ.data[1];
        this.data[10] *= vectorXYZ.data[2];
        
        return this;
    }
    
    matObj.prototype.setProjection = function (angle, ar, zMin, zMax) {
        var ang = Math.tan((angle*.5)*Math.PI/180);

        this.data = [
           0.5/ang, 0 , 0, 0,
           0, 0.5*ar/ang, 0, 0,
           0, 0, -(zMax+zMin)/(zMax-zMin), -1,
           0, 0, (-2*zMax*zMin)/(zMax-zMin), 0 
        ];
        
        return this;
    }
    
    matObj.prototype.zoom = function (zoomValue) {
        this.data[14] += zoomValue;
        
        return this;
    }
    
    matObj.prototype.setUniform = function (programID, uniformName) {
        var matLoc = gl.getUniformLocation(programID, uniformName);
        gl.uniformMatrix4fv(matLoc, false, this.getData());
    }
    
    matObj.prototype.copy = function () {
        var copyMatrix = new matObj ();
        
        for (var idx = 0; idx < 16; idx++) {
            copyMatrix.data[idx] = this.data[idx];
        }
        
        return copyMatrix;
    }
    
    matObj.prototype.at = function (row, col) {
        return this.data[row * 4 + col];
    }
    
    matObj.prototype.add = function (row, col, val) {
        this.data[row * 4 + col] += val;
    }
    
    matObj.prototype.multiply = function (rightMat) {
        var copyThis = this.copy();
        this.clear();
        
        for (var row = 0; row < 4; row++) {
            for (var col = 0; col < 4; col++) {
                for (var idx = 0; idx < 4; idx++) {
                    
                    this.add (row, col, copyThis.at(row, idx) * rightMat.at(idx, col));
                    
                }
            }
        }
        return this;
    }
    
    return matObj;
}) ();