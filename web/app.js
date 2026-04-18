var switchCount = 0
var lightCount = 0

const lightsContainer = document.getElementById("lightContainer")
const switchesContainer = document.getElementById("switchContainer")
const lights = []
const switches = []

// to be called from C
function jsReadSwitch(switchNumber) {
    const switchToRead = switches[switchNumber]
    const switchInputElement = switchToRead.querySelector(".switch")
    return switchInputElement.checked
}

// to be called from C
function jsSetLightState(lightNumber, value) {
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
function jsAddLights(lightsContainerToAddCount) {
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
function jsAddSwitches(switchesContainerToAddCount) {
    for (var i = switchCount; i < switchCount + switchesContainerToAddCount; ++i) {
        const newSwitch = document.createElement("div")
        newSwitch.innerHTML = `
            <input class="switch" type="checkbox" id="switch_${i}">
            <label for="switch_${i}" class="switch_label"></label>
        `
        switchesContainer.appendChild(newSwitch)
        switches.push(newSwitch)
    }
    switchCount += switchesContainerToAddCount
}

jsAddLights(6)
jsAddSwitches(5)

setTimeout(() => {
    jsSetLightState(0, 1)
    jsSetLightState(2, 1)
}, 1500)

setTimeout(() => {
    const value = jsReadSwitch(2)
    console.log(`switch state read: ${value}`)
    jsSetLightState(2, 0)
}, 4500)
