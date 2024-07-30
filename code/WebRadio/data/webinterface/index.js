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
const rmCls = (_el, _cls) => _el.classList.remove(_cls)

function isValidUrl(string) {
    try {
      new URL(string)
      return true
    } catch (_) {
      return false
    }
}


/**
 * INIT
 */
document.addEventListener('DOMContentLoaded', async () => {
    loadStationList()
    loadSavedNetworksList()
    setInterval(loadInfoAndWifiStatusData, 2000)
    // loadInfoAndWifiStatusData()
})


async function loadList(
    _list_el,
    list_item_el,
    template_el,
    data_function
) {
    _list_el.innerHTML = template_el.innerHTML
    const stations_list = await data_function()
    _list_el.innerHTML = ''
    const template_html = list_item_el.innerHTML
    stations_list.forEach((station, index) => {
        let html = template_html
        Object.keys(station).forEach((key) => {
            html = html.replaceAll(`{{ ${key} }}`, station[key]);
        })
        html = html.replaceAll('{{ index }}', index);
        const _new_el = document.createElement("div")
        _new_el.innerHTML = html
        _list_el.appendChild(_new_el.firstElementChild)
    })
}


async function loadStationList() {
    const _el = $('#radio-stations-list')
    await loadList(
        _el,
        $('#template-radio-station-list-item'),
        $('#template-list-loader'),
        () => serverGetStationList()
    )

    const _list = _el.querySelectorAll('.list-item')
    if (_list.length == 0) {
        return
    }
    _list[0].querySelector('.button-up').disabled = true
    _list[_list.length - 1].querySelector('.button-down').disabled = true
}


async function addStation(btn) {

    const _name_input = $('#input-station-name')
    const _host_input = $('#input-station-host')
        
    const name = _name_input.value
    const host = _host_input.value

    if (name.length == 0) {
        addCls(_name_input, 'error')
        return
    }

    if (!isValidUrl(host)) {
        addCls(_host_input, 'error')
        return
    }

    if (name.length > STATION_NAME_MAX_LENGTH) {
        addCls(_name_input, 'error')
        return
    }

    if (host.length > STATION_HOST_MAX_LENGTH) {
        addCls(_host_input, 'error')
        return
    }
    
    rmCls(_name_input, 'error')
    rmCls(_host_input, 'error')

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


async function editStation(elemnent) {
    elemnent.parentElement.parentElement.remove()
}

async function stationMoveUp(_el) {
    const index = Number(_el.dataset.index)
    await serverStationMoveUp(index)
    await loadStationList()
}

async function stationMoveDown(_el) {
    const index = Number(_el.dataset.index)
    await serverStationMoveDown(index)
    await loadStationList()
}


/**
 * Wifi management
 */
async function loadAvailableNetworksList() {
    await loadList(
        $('#available-networks-list'),
        $('#template-available-networks-list-item'),
        $('#template-list-loader'),
        () => serverGetAvailableNetworksList()
    )
}

async function loadSavedNetworksList() {
    await loadList(
        $('#saved-networks-list'),
        $('#template-saved-networks-list-item'),
        $('#template-list-loader'),
        () => serverGetSavedNetworksList()
    )
}

async function addAndConnectNetwork() {
    const _el = $('#wifi-credentials-manager')
    addCls($(".main-actions", _el), 'hidden')
    rmCls($(".available-networks-card", _el), 'hidden')
    rmCls($(".available-networks-card .step-1", _el), 'hidden')
    await loadAvailableNetworksList()
}

async function connectToNetwork(element) {
    const _el = $('#wifi-credentials-manager')
    addCls($(".main-actions", _el), 'hidden')
    rmCls($(".available-networks-card", _el), 'hidden')
    addCls($(".available-networks-card .step-1", _el), 'hidden')
    rmCls($(".available-networks-card .step-2", _el), 'hidden')
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
    rmCls($(".main-actions", _el), 'hidden')
    addCls($(".available-networks-card", _el), 'hidden')
    addCls($(".available-networks-card .step-1", _el), 'hidden')
    addCls($(".available-networks-card .step-2", _el), 'hidden')
}


async function saveNetwork(btn) {
    const _el = $('#wifi-credentials-manager')
    const _name_input = $("#input-ssid-name", _el)
    const _password_input = $("#input-ssid-password", _el)
    const name = _name_input.value
    const password = _password_input.value

    if (name.length == 0) {
        addCls(_name_input, 'error')
        return
    }
    
    if (password.length == 0) {
        addCls(_password_input, 'error')
        return
    }
    
    rmCls(_name_input, 'error')
    rmCls(_password_input, 'error')

    buttonSetLoadingState(btn, true)
    await serviceAddSavedNetwork({ssid: name, password})
    buttonSetLoadingState(btn, false)

    await loadSavedNetworksList()
    await resetCreatingSavedWifi()
}

async function removeSavedNetwork(btn) {
    const ssid = btn.dataset.ssid
    buttonSetLoadingState(btn, true)
    await serverRemoveSavedNetwork(ssid)
    buttonSetLoadingState(btn, false)
    await loadSavedNetworksList()
}

async function refreshAvailableNetworks(btn) {
    buttonSetLoadingState(btn, true)
    await loadAvailableNetworksList()
    buttonSetLoadingState(btn, false)
}


/**
 * Info
 */
async function refreshInfo(btn) {
    buttonSetLoadingState(btn, true)
    await loadInfoAndWifiStatusData()
    buttonSetLoadingState(btn, false)
}


async function loadInfoAndWifiStatusData() {
    const info = await serverGetInfo()
    const _el_block_wifi_status = $('.block-wifi-status')
    const _el_block_info = $('.block-info')
    $('.connection-status', _el_block_wifi_status).textContent = info.is_connecting_to_wifi
        ? 'Connecting...'
        : info.is_connected_to_wifi ? 'Connected' : 'Disconnected'
    $('.wifi-ssid', _el_block_wifi_status).textContent = info.connected_to_ssid ==  '' ?  'None' : info.connected_to_ssid
    $('.local-ip', _el_block_wifi_status).textContent = info.local_ip ==  '' ? 'None' : info.local_ip

    $('.temperature', _el_block_info).textContent = info.temperature.toFixed(2)
    $('.pressure', _el_block_info).textContent = info.pressure.toFixed(2)
    $('.free-memory', _el_block_info).textContent = info.total_free_bytes
    $('.min-free-memory', _el_block_info).textContent = info.minimum_free_bytes

    $('.time', _el_block_info).textContent = info.date_time_configured
        ? `${formatTime(info.date_time)}`
        : 'not configured'
}


function formatTime(date_time_string) {
    const date = new Date(date_time_string)
    const hours = date.getHours().toString().padStart(2, '0')
    const minutes = date.getMinutes().toString().padStart(2, '0')
    return `${hours}:${minutes}`;
}