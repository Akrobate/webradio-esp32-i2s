


async function serverGetStationList() {
    try {
        const result = await fetch('/api/streams',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetStationList - Error:', error)
        return []
    }
}

async function serverGetAvailableNetworksList() {
    try {
        const result = await fetch('/api/available-networks',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetAvailableNetworksList - Error:', error)
        return []
    }
}


async function serverGetSavedNetworksList() {
    try {
        const result = await fetch('/api/credentials',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetSavedNetworksList - Error:', error)
        return []
    }
}


async function serviceAddSavedNetwork(network) {
    const {
        ssid,
        password,
    } = network
    try {
        let form_data = new FormData()
        form_data.append('ssid', ssid)
        form_data.append('password', password)
        await fetch('/api/credentials',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serviceAddSavedNetwork - Error:', error)
    }
}


async function serverRemoveSavedNetwork(ssid) {
    try {
        let form_data = new FormData()
        form_data.append('ssid', ssid)
        await fetch('/api/credentials',
            {
                body: form_data,
                method: 'DELETE',
            }
        )
    } catch (error) {
        console.log('serverRemoveSavedNetwork - Error:', error)
    }
}
