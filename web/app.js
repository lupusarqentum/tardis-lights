var switchCount = 0
var lightCount = 0
const lights = document.getElementById("lightContainer")
const switches = document.getElementById("switchContainer")

function addLights(lightsToAddCount) {
    for (var i = lightCount; i < lightCount + lightsToAddCount; ++i) {
        const newLight = document.createElement("div")
        newLight.innerHTML = `
            <div class="lightBorder", style="display: flex; justify-content: center; align-items: center;">
                <div class="lightFuck"></div>
                <div class="lightOff light", id="light_${i}"></div>
                <div class="lightFuck"></div>
            </div>
        `
        lights.appendChild(newLight)
    }
    lightCount += lightsToAddCount
}

function addSwitches(switchesToAddCount) {
    for (var i = switchCount; i < switchCount + switchesToAddCount; ++i) {
        const newSwitch = document.createElement("div")
        newSwitch.innerHTML = `
            <input class="switch" type="checkbox" id="switch_${i}">
            <label for="switch_${i}" class="switch_label"></label>
        `
        switches.appendChild(newSwitch)
    }
    switchCount += switchesToAddCount
}

addLights(6)
addSwitches(5)
setTimeout(() => {
    document.getElementById("light_1").className = "lightOn light"
}, 2000);
