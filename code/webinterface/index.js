const STATION_NAME_MAX_LENGTH = 30
const STATION_HOST_MAX_LENGTH = 1000

let info_data = {}

/**
 * Generic methods
 */

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


function formatTime(date_time_string) {
    const date = new Date(date_time_string)
    const hours = date.getHours().toString().padStart(2, '0')
    const minutes = date.getMinutes().toString().padStart(2, '0')
    return `${hours}:${minutes}`;
}

async function loadList(
    _list_el,
    list_item_el,
    template_el,
    data_function
) {
    _list_el.innerHTML = template_el.innerHTML
    const data_list = await data_function()
    _list_el.innerHTML = ''
    const template_html = list_item_el.innerHTML
    data_list.forEach((data, index) => {
        let html = template_html
        Object.keys(data).forEach((key) => {
            html = html.replaceAll(`{{ ${key} }}`, data[key]);
        })
        html = html.replaceAll('{{ index }}', index);
        const _new_el = document.createElement("div")
        _new_el.innerHTML = html
        _list_el.appendChild(_new_el.firstElementChild)
    })
}


/**
 * INIT
 */
document.addEventListener('DOMContentLoaded', async () => {
    await loadInfoAndWifiStatusData()
    loadStationList()
    loadSavedNetworksList()
    initDateTimeConfigurationManager()
    setInterval(loadInfoAndWifiStatusData, 2000)
})


/**
 * Stations management
 */
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


async function showAddFormStation() {
    const _el = $('#station-manager')
    const _add_form_trigger = $('.add-trigger', _el)
    const _add_form = $('.add-form', _el)
    const _add_station = $('.add-station', _el)

    rmCls(_add_form, 'hidden')
    addCls(_add_form_trigger, 'hidden')
    rmCls(_add_station, 'hidden')
}


async function showEditFormStation(btn) {
    resetHideAddFormStation()
    const index = Number(btn.dataset.index)
    const name = btn.dataset.name
    const host = btn.dataset.host

    const {
        _name_input,
        _host_input,
        _index_input,
        _station_manager_el: _el
    } = extractFormInputsStation();

    const _add_form_trigger = $('.add-trigger', _el)
    const _add_form = $('.add-form', _el)
    const _edit_station = $('.edit-station', _el)

    _name_input.value = name
    _host_input.value = host
    _index_input.value = index

    rmCls(_add_form, 'hidden')
    addCls(_add_form_trigger, 'hidden')
    rmCls(_edit_station, 'hidden')
}


async function resetHideAddFormStation() {
    const {
        _name_input,
        _host_input,
        _index_input,
        _station_manager_el: _el,
    } = extractFormInputsStation();

    const _add_form_trigger = $('.add-trigger', _el)
    const _add_form = $('.add-form', _el)
    const _add_station = $('.add-station', _el)
    const _edit_station = $('.edit-station', _el)

    addCls(_add_station, 'hidden')
    addCls(_edit_station, 'hidden')
    addCls(_add_form, 'hidden')
    rmCls(_add_form_trigger, 'hidden')

    _name_input.value = ''
    _host_input.value = ''
    _index_input.value = ''

    _name_input.disabled = false
    _host_input.disabled = false
    rmCls(_name_input, 'error')
    rmCls(_host_input, 'error')
}


async function addEditStation(btn) {

    const {
        _name_input,
        _host_input,
        _index_input,
        name,
        host,
        index,
    } = extractFormInputsStation();

    console.log("index........",_index_input.value);

    const is_editing =_index_input.value !== ''

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

    if (is_editing) {
        await serverEditStation(index, {name, host})
    } else {
        await serverAddStation({name, host})
    }

    buttonSetLoadingState(btn, false)
    resetHideAddFormStation()
    await loadStationList()
}


async function deleteStation(btn) {
    const index = btn.dataset.index
    buttonSetLoadingState(btn, true)
    await serverRemoveStation(index)
    buttonSetLoadingState(btn, true)
    await loadStationList()
}


async function MoveUpStation(_el) {
    const index = Number(_el.dataset.index)
    await serverMoveUpStation(index)
    await loadStationList()
}

async function MoveDownStation(_el) {
    const index = Number(_el.dataset.index)
    await serverMoveDownStation(index)
    await loadStationList()
}


function extractFormInputsStation() {
    const _el = $('#station-manager')
    const _name_input = $('#input-station-name', _el)
    const _host_input = $('#input-station-host', _el)
    const _index_input = $('#input-station-index', _el)
    
    const name = _name_input.value
    const host = _host_input.value
    const index = Number(_index_input.value)

    return {
        _station_manager_el: _el,
        _name_input,
        _host_input,
        _index_input,
        name,
        host,
        index,
    }
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
    info_data = await serverGetInfo()

    const info = {
        ...info_data,
    }

    const _el_block_info_primary = $('.block-info-primary')
    const _el_block_info_secondary = $('.block-info-secondary')
    $('.connection-status', _el_block_info_secondary).textContent = info.is_connecting_to_wifi
        ? 'Connecting...'
        : info.is_connected_to_wifi ? 'Connected' : 'Disconnected'
    $('.wifi-ssid', _el_block_info_secondary).textContent = info.connected_to_ssid ==  '' ?  'None' : info.connected_to_ssid
    $('.local-ip', _el_block_info_secondary).textContent = info.local_ip ==  '' ? 'None' : info.local_ip

    $('.temperature', _el_block_info_primary).textContent = info.temperature.toFixed(2)
    $('.pressure', _el_block_info_primary).textContent = info.pressure.toFixed(2)
    $('.free-memory', _el_block_info_secondary).textContent = info.total_free_bytes
    $('.min-free-memory', _el_block_info_secondary).textContent = info.minimum_free_bytes

    $('.time', _el_block_info_primary).textContent = info.date_time_configured
        ? `${formatTime(info.date_time)}`
        : 'not configured'
}

/**
 * DateTimeConfigurationManager
 */
async function initDateTimeConfigurationManager() {

    const daylight_offset_label_list = [
        {
            value: -3600 * 2,
            label: '-2 hours'
        },
        {
            value: -3600,
            label: '-1 hour'
        },
        {
            value: 0,
            label: 'None'
        },
        {
            value: 3600,
            label: '+1 hour'
        },
        {
            value: 3600 * 2,
            label: '+2 hours'
        },
    ]

    const gmt_offset_label_list = Array.from({length: 27}).map((_, index) => {
        const hours = index - 12
        return {
            value: 3600 * hours,
            label: hours === 1 || hours === -1 ? `GMT ${hours}` : hours > 0 ? `GMT +${hours}` : `GMT ${hours}`
        }
    })

    const _el = $('#date-time-configuration-manager')

    const _el_gmt_offset_sec = $('#gmt-offset-sec', _el)
    const _el_daylight_offset_sec = $('#daylight-offset-sec', _el)
    const _el_ntp_server_host = $('#ntp-server-host', _el)

    _el_gmt_offset_sec.innerHTML = gmt_offset_label_list.map((item) => `<option value="${item.value}">${item.label}</option>`).join(' ')
    _el_daylight_offset_sec.innerHTML = daylight_offset_label_list.map((item) => `<option value="${item.value}">${item.label}</option>`).join(' ')
    _el_ntp_server_host.value = info_data.ntp_server_host

    _el_gmt_offset_sec.value = info_data.gmt_offset_sec
    _el_daylight_offset_sec.value = info_data.daylight_offset_sec

}


async function saveConfiguration(btn) {

    const _el = $('#date-time-configuration-manager')
    const _el_gmt_offset_sec = $('#gmt-offset-sec', _el)
    const _el_daylight_offset_sec = $('#daylight-offset-sec', _el)
    const _el_ntp_server_host = $('#ntp-server-host', _el)

    const gmt_offset_sec = Number(_el_gmt_offset_sec.value)
    const daylight_offset_sec = Number(_el_daylight_offset_sec.value)
    const ntp_server_host = _el_ntp_server_host.value

    buttonSetLoadingState(btn, true)
    await serverSaveConfigurations({gmt_offset_sec, daylight_offset_sec, ntp_server_host})
    buttonSetLoadingState(btn, false)

}

