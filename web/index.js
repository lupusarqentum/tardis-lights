var switchCount = 0
var lightCount = 0

const lightsContainer = document.getElementById("lightContainer")
const switchesContainer = document.getElementById("switchContainer")
const lights = []
const switches = []

// to be called from C
function readSwitch(switchNumber) {
    const switchToRead = switches[switchNumber]
    const switchInputElement = switchToRead.querySelector(".switch")
    return switchInputElement.checked
}

// to be called from C
function setLightState(lightNumber, value) {
    const lightBase = lights[lightNumber]
    const light = lightBase.querySelector(".light")
    if (value == 1) {
        light.classList.add("lightOn")
        light.classList.remove("lightOff")
    } else if (value == 0) {
        light.classList.add("lightOff")
        light.classList.remove("lightOn")
    } else {
        console.log(`error: unrecognized value given to set to a light: ${value}`)
    }
}

// to be called from C
function addLights(lightsContainerToAddCount) {
    for (var i = lightCount; i < lightCount + lightsContainerToAddCount; ++i) {
        const newLight = document.createElement("div")
        newLight.innerHTML = `
            <div class="lightBorder", style="display: flex; justify-content: center; align-items: center;">
                <div class="lightFuck"></div>
                <div class="lightOff light", id="light_${i}"></div>
                <div class="lightFuck"></div>
            </div>
        `
        lightsContainer.appendChild(newLight)
        lights.push(newLight)
    }
    lightCount += lightsContainerToAddCount
}

// to be called from C
function addSwitches(switchesContainerToAddCount) {
    for (var i = switchCount; i < switchCount + switchesContainerToAddCount; ++i) {
        const newSwitch = document.createElement("div")
        newSwitch.innerHTML = `
            <input class="switch" type="checkbox" onchange="handleSwitchToggle(this)" id="switch_${i}">
            <label for="switch_${i}" class="switch_label"></label>
        `
        switchesContainer.appendChild(newSwitch)
        switches.push(newSwitch)
    }
    switchCount += switchesContainerToAddCount
}

// to be called from C
function jsLog(message_pointer, message_length) {
    const shit = new Uint8Array(instance.exports.memory.buffer, message_pointer, message_length)
    const shit2 = new TextDecoder().decode(shit)
    console.log(`fucking string ${shit2}`)
}

function handleSwitchToggle(_) {
    if (instance != undefined) {
        instance.exports.loop()
    }
}

async function init() {
    const imports = {
        env: {
            jsReadSwitch: readSwitch,
            jsSetLightState: setLightState,
            jsAddLights: addLights,
            jsAddSwitches: addSwitches,
            jsLog: jsLog
        }
    };

    const { instance } = await WebAssembly.instantiateStreaming(fetch("./yiff.wasm"), imports);
    globalThis.instance = instance
    instance.exports.setup()
}

init()
