








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


async function serverRemoveSavedNetwork(network_index) {
    const {
        ssid,
    } = network
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
        console.log('serviceAddSavedNetwork - Error:', error)
    }
}
