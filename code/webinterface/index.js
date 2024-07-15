const STATION_NAME_MAX_LENGTH = 30
const STATION_HOST_MAX_LENGTH = 1000

function $(selector, element = document) {
    return element.querySelector(selector)
}

function buttonSetLoadingState(btn, state) {
    btn.disabled = state
    if (state) {
        btn.dataset.originalText = btn.textContent
        btn.textContent = 'Loading...'
    } else {
        btn.textContent = btn.dataset.originalText
    }
}

const addCls = (_el, _cls) => _el.classList.add(_cls)


/**
 * INIT
 */
document.addEventListener('DOMContentLoaded', async () => {
    loadStationList()
    loadSavedNetworksList()
})


/**
 * Station management
 */
async function _loadStationList() {
    const _list_el = $('#radio-stations-list')
    _list_el.innerHTML = $('#template-list-loader').innerHTML
    const stations_list = await serverGetStationList()
    _list_el.innerHTML = ''
    stations_list.forEach((station, index) => {
        const _new_el = $('#template-radio-station-list-item').content.cloneNode(true)
        $('.title', _new_el).textContent = station.name
        $('.subtitle', _new_el).textContent = station.host
        $('.list-item', _new_el).dataset.index = index
        _list_el.appendChild(_new_el)
    })
}

/**
 * Station management
 */
async function loadStationList() {
    const _list_el = $('#radio-stations-list')
    _list_el.innerHTML = $('#template-list-loader').innerHTML
    const stations_list = await serverGetStationList()
    _list_el.innerHTML = ''
    stations_list.forEach((station, index) => {
        let html = $('#template-radio-station-list-item').innerHTML;
        Object.keys(station).forEach((key) => {
            html = html.replaceAll(`{{ ${key} }}`, station[key]);
        })
        html = html.replaceAll('{{ index }}', index);
        const placeholder = document.createElement("div")
        placeholder.innerHTML = html
        _list_el.appendChild(placeholder.firstElementChild)
    })
}



async function addStation(btn) {

    const _name_input = $('#input-station-name')
    const _host_input = $('#input-station-host')
        
    const name = _name_input.value
    const host = _host_input.value

    if (name.length == 0) {
        _name_input.classList.add('error')
        return
    }

    if (!isValidUrl(host)) {
        _host_input.classList.add('error')
        return
    }

    if (name.length > STATION_NAME_MAX_LENGTH) {
        _name_input.classList.add('error')
        return
    }

    if (host.length > STATION_HOST_MAX_LENGTH) {
        _host_input.classList.add('error')
        return
    }
    
    _name_input.classList.remove('error')
    _host_input.classList.remove('error')

    _name_input.disabled = true
    _host_input.disabled = true

    buttonSetLoadingState(btn, true)
    await serverAddStation({name, host})
    _host_input.value = ''
    _name_input.value = ''
    _name_input.disabled = false
    _host_input.disabled = false
    buttonSetLoadingState(btn, false)

    await loadStationList()

}


async function deleteStation(btn) {
    const index = btn.dataset.index

    buttonSetLoadingState(btn, true)
    await serverRemoveStation(index)
    buttonSetLoadingState(btn, true)

    await loadStationList()
}


function editStation(elemnent) {
    elemnent.parentElement.parentElement.remove()
}


function isValidUrl(string) {
    try {
      new URL(string)
      return true
    } catch (_) {
      return false
    }
}


/**
 * Wifi management
 */

let wifi_management_add_and_connect_state = false


async function loadAvailableNetworksList() {
    const _list_el = $('#available-networks-list')
    _list_el.innerHTML = $('#template-list-loader').innerHTML
    const available_networks_list_list = await serverGetAvailableNetworksList()
    _list_el.innerHTML = ''
    available_networks_list_list.forEach((station, index) => {
        const _new_el = $('#template-available-networks-list-item').content.cloneNode(true)
        $('.title', _new_el).textContent = station.ssid
        $('.list-item', _new_el).dataset.index = index
        $('button', _new_el).dataset.ssid = station.ssid
        _list_el.appendChild(_new_el)
    })
}


async function loadSavedNetworksList() {
    const _list_el = $('#saved-networks-list')
    _list_el.innerHTML = $('#template-list-loader').innerHTML
    const saved_networks_list = await serverGetSavedNetworksList()

    _list_el.innerHTML = ''
    saved_networks_list.forEach((station, index) => {
        const _new_el = $('#template-saved-networks-list-item').content.cloneNode(true)
        $(".title", _new_el).textContent = station.ssid
        $(".list-item", _new_el).dataset.index = index
        _list_el.appendChild(_new_el)
    })
}


async function addAndConnectNetwork() {
    const _el = $('#wifi-credentials-manager')
    $(".main-actions", _el).classList.add('hidden')
    $(".available-networks-card", _el).classList.remove('hidden')
    $(".available-networks-card .step-1", _el).classList.remove('hidden')
    await loadAvailableNetworksList()
}


async function connectToNetwork(element) {
    const _el = $('#wifi-credentials-manager')
    $(".main-actions", _el).classList.add('hidden')
    $(".available-networks-card", _el).classList.remove('hidden')
    $(".available-networks-card .step-1", _el).classList.add('hidden')
    $(".available-networks-card .step-2", _el).classList.remove('hidden')
    $("#input-ssid-password", _el).value = ''
    $("#input-ssid-name", _el).value = ''

    if (element) {
        $('#input-ssid-name', _el).value = element.dataset.ssid
        $('#input-ssid-name', _el).disabled = true
    } else {
        $('#input-ssid-name', _el).value = ''
        $('#input-ssid-name', _el).disabled = false    
    }
}


async function resetCreatingSavedWifi() {
    const _el = $('#wifi-credentials-manager')
    $(".main-actions", _el).classList.remove('hidden')
    $(".available-networks-card", _el).classList.add('hidden')
    $(".available-networks-card .step-1", _el).classList.add('hidden')
    $(".available-networks-card .step-2", _el).classList.add('hidden')
}


async function saveNetwork(btn) {
    const _el = $('#wifi-credentials-manager')
    const _name_input = $("#input-ssid-name", _el)
    const _password_input = $("#input-ssid-password", _el)
    const name = _name_input.value
    const password = _password_input.value

    if (name.length == 0) {
        _name_input.classList.add('error')
        return
    }
    
    if (password.length == 0) {
        _password_input.classList.add('error')
        return
    }
    
    _name_input.classList.remove('error')
    _password_input.classList.remove('error')

    buttonSetLoadingState(btn, true)
    await serviceAddSavedNetwork({ssid: name, password})
    buttonSetLoadingState(btn, false)

    await loadSavedNetworksList()
    await resetCreatingSavedWifi()
}

async function removeSavedNetwork(btn) {
    const index = btn.dataset.index
    buttonSetLoadingState(btn, true)
    await serverRemoveSavedNetwork(index)
    buttonSetLoadingState(btn, false)
    await loadSavedNetworksList()
}

async function refreshAvailableNetworks(btn) {
    buttonSetLoadingState(btn, true)
    await loadAvailableNetworksList()
    buttonSetLoadingState(btn, false)
}
