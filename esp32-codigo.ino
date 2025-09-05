//Equipe:
//Raphael Aaron - 564067
//Felipe Catto - 562106
//Kimberly Kristina - 564080
//Laura Dantas - 564064

#include <WiFi.h>
#include <PubSubClient.h>

// Configurando as variáveis
const char* default_SSID = "sua_rede_wifi"; // Inserir nome da rede Wi-Fi
const char* default_PASSWORD = "sua_senha_wifi"; // Inserir senha da rede Wi-Fi
const char* default_BROKER_MQTT = "ip_host_fiware"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; // Tópico MQTT de envio de luminosidade
const char* default_ID_MQTT = "fiware_001"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "lamp001";

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID); // Variável de SSID da rede, inicializada com valor padrão
char* PASSWORD = const_cast<char*>(default_PASSWORD); // Variável de senha da rede, inicializada com valor padrão
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT); // Endereço do Broker MQTT
int BROKER_PORT = default_BROKER_PORT; // Porta do Broker MQTT
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE); // Variável de tópico de assinatura
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1); // Variável de tópico de publicação 1
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2); // Variável de tópico de publicação 2
char* ID_MQTT = const_cast<char*>(default_ID_MQTT); // Variável do ID MQTT
int D4 = default_D4; // Variável do pino de saída (LED onboard)

WiFiClient espClient; // Cliente Wi-Fi usado para conexão
PubSubClient MQTT(espClient); // Cliente MQTT usando conexão Wi-Fi
char EstadoSaida = '0'; // Estado inicial do LED (desligado)

// Inicializa comunicação serial
void initSerial() {
    Serial.begin(115200); // Configura baud rate da porta serial
}

// Inicializa conexão Wi-Fi
void initWiFi() {
    delay(10); // Pequeno delay para estabilização
    Serial.println("------Conexao WI-FI------"); // Mensagem no monitor serial
    Serial.print("Conectando-se na rede: "); // Mostra a rede alvo
    Serial.println(SSID); // Exibe o SSID configurado
    Serial.println("Aguarde"); // Mensagem de aguardo
    reconectWiFi(); // Chama função para conectar ou reconectar ao Wi-Fi
}

// Inicializa cliente MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Define servidor e porta MQTT
    MQTT.setCallback(mqtt_callback); // Define função de callback para mensagens recebidas
}

// Função principal de configuração
void setup() {
    InitOutput(); // Inicializa saída (LED com piscadas)
    initSerial(); // Inicia comunicação serial
    initWiFi(); // Conecta ao Wi-Fi
    initMQTT(); // Configura conexão MQTT
    delay(5000); // Aguarda 5s antes de publicar primeira mensagem
    MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica mensagem inicial de status
}

// Função principal de execução contínua
void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica e reconecta Wi-Fi/MQTT se necessário
    EnviaEstadoOutputMQTT(); // Envia estado do LED ao broker
    handleLuminosity(); // Lê luminosidade e envia para o broker
    MQTT.loop(); // Mantém cliente MQTT ativo
}

// Conecta ou reconecta ao Wi-Fi
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED) // Se já está conectado, sai da função
        return;
    WiFi.begin(SSID, PASSWORD); // Inicia conexão Wi-Fi
    while (WiFi.status() != WL_CONNECTED) { // Aguarda conexão
        delay(100);
        Serial.print("."); // Imprime pontos enquanto espera
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede "); // Mensagem de sucesso
    Serial.print(SSID); // Mostra SSID conectado
    Serial.println("IP obtido: "); 
    Serial.println(WiFi.localIP()); // Mostra IP obtido pelo dispositivo

    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}

// Função de callback MQTT (executada ao receber mensagem)
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg; // Armazena mensagem recebida
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i]; // Converte payload para char
        msg += c; // Concatena no objeto String
    }
    Serial.print("- Mensagem recebida: "); // Mostra mensagem recebida
    Serial.println(msg);

    // Forma o padrão de tópico esperado para ligar/desligar
    String onTopic = String(topicPrefix) + "@on|"; 
    String offTopic = String(topicPrefix) + "@off|";

    // Se a mensagem recebida for comando de ligar
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH); // Liga LED
        EstadoSaida = '1'; // Atualiza estado
    }

    // Se a mensagem recebida for comando de desligar
    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW); // Desliga LED
        EstadoSaida = '0'; // Atualiza estado
    }
}

// Verifica se há conexão com Wi-Fi e MQTT
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected()) // Se não estiver conectado ao broker
        reconnectMQTT(); // Tenta reconectar
    reconectWiFi(); // Garante que Wi-Fi esteja conectado
}

// Envia estado do LED ao broker
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') { // Se LED ligado
        MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica mensagem de ligado
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') { // Se LED desligado
        MQTT.publish(TOPICO_PUBLISH_1, "s|off"); // Publica mensagem de desligado
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!"); // Confirma envio
    delay(1000); // Delay para não sobrecarregar publicações
}

// Inicializa saída (LED onboard)
void InitOutput() {
    pinMode(D4, OUTPUT); // Configura pino como saída
    digitalWrite(D4, HIGH); // Liga LED inicialmente
    boolean toggle = false; // Variável de controle de piscadas

    for (int i = 0; i <= 10; i++) { // Pisca LED 10 vezes
        toggle = !toggle; // Alterna estado
        digitalWrite(D4, toggle); // Atualiza LED
        delay(200); // Tempo entre piscadas
    }
}

// Reconecta ao broker MQTT caso esteja desconectado
void reconnectMQTT() {
    while (!MQTT.connected()) { // Enquanto não estiver conectado
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT); // Exibe endereço do broker
        if (MQTT.connect(ID_MQTT)) { // Tenta conectar usando ID
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); // Se conectou, inscreve no tópico
        } else {
            Serial.println("Falha ao reconectar no broker."); // Erro de conexão
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000); // Aguarda 2 segundos antes de tentar novamente
        }
    }
}

// Lê sensor de luminosidade e envia valor ao broker
void handleLuminosity() {
    const int potPin = 34; // Pino analógico usado para leitura
    int sensorValue = analogRead(potPin); // Lê valor do sensor (0 a 4095)
    int luminosity = map(sensorValue, 0, 4095, 0, 100); // Converte para escala 0-100%
    String mensagem = String(luminosity); // Converte valor para string
    Serial.print("Valor da luminosidade: "); // Mostra valor no monitor serial
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str()); // Publica valor de luminosidade no broker
}
