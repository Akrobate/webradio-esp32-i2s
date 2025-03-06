const axios = require('axios');


// URL cible pour le test de charge
const url_list = [
    'http://192.168.1.25/',
    'http://192.168.1.25/index.js.gz',
    'http://192.168.1.25/repository.js.gz',
    'http://192.168.1.25/style.css.gz',
];

// Nombre de requêtes à envoyer pour le test de charge
const REQUEST_COUNT = 10000;

// Fonction pour envoyer une seule requête
async function sendRequest(url) {
  try {
    const startTime = Date.now();  // Enregistrer le temps de départ
    const response = await axios.get(url);
    const endTime = Date.now();    // Enregistrer le temps de fin

    // Afficher le statut et le temps pris pour chaque requête
    console.log(`Request ${url} - Status: ${response.status}, Time: ${endTime - startTime} ms`);
  } catch (error) {
    // Gérer les erreurs lors de la requête
    console.log(`Request ${url} failed: ${error.message}`);
  }
}

// Fonction pour exécuter le test de charge
async function runLoadTest() {


  // Attendre que toutes les requêtes soient terminées
  for (let i = 0; i < REQUEST_COUNT; i++) {
        const promises = [];

        // Envoyer plusieurs requêtes simultanées
        for (let url of url_list) {
        promises.push(sendRequest(url));
        }

        const startTime = Date.now();
        await Promise.all(promises);
        const endTime = Date.now();    // Enregistrer le temps de fin
        
        console.log(`==========> Request ${i} - Time: ${endTime - startTime} ms`);

  }
  console.log(`Finished sending ${REQUEST_COUNT} requests.`);
}

// Lancer le test de charge
runLoadTest();

