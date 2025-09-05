# Fiware - Projeto Lamp
> Fiware: seus componentes básicos e uso com ESP32

# Componentes Básicos
> O Fiware consiste em uma plataforma de código aberto, a qual permite interagir diferentes sistemas e dispositivos em apenas um local.
> As informações são, portanto, armazenadas em um local em comum.

### NGSI - Next Generation Service Interface
  O NGSI trata-se de um padronizador de linguagem. Consiste em um padrão de comunicação pra trocar informaç~ies entre sistemas. O mesmo:
- Define como os dados devem ser organizados
- Define como você pede ou envia informações

### JSON - JavaScript Object Notation
  O NGSI utiliza o formato json, o qual é definido por um formato de texto para guardar e trocar informações entre sistemas.

### Orion Context Broker
  O Context Broker é o componente central de qualquer sistema baseado em NGSI. Consiste em um 'local' que as informações de contexto são armazenadas, gerenciadas e  distribuídas.
> Informações de contexto são dados que descrevem o atual estado de algo.
> As informações de contexto são alteráveis e, desse modo, precisam ser acessadas por diversos sistemas.
  As principais ações do Context Broker são:
  - Recebe dados
  - Armazena dados de contexto
  - Permite consultas
  - Alerta sobre mudanças

> O Context Broker não faz parte do NGSI, mas ele segue as regras do mesmo, com o intúito de gerenciar as informações.

### STH-Comet - Shot Term Historic Comet
  Componente do Fiware que trabalha em conjunto com o Orion Context Broker. O STH - Comet armazena o histórico de informações de contexto.
  Sua relevância de deve pois o Context Broker armazena apenas o estado atual de componentes. O STH - Comet recebe as notificações de mudanças pelo Orion Context Broker e armazena os dados (em MangoDB, usualmente), permitindo consultas dos dados para futuras análises.
> MangoDG: banco de dados NoSQL (bancos não relacionais).

### MQTT - Message Quering Telemetry Transport
  Protocolo de comunicação leve, para enviar mensagens entre dispositivos.
  Dentro do Fiware, há o Agent MQTT, o qual refere-se ao componente que realiza a ponte entre dispositivos que utilizam MQTT e Orion Context Broker, esse último que segue o padrão NGSI.
  Dentre suas ações, temos:
  - Escutar mensagens MQTT de dispositivos
  - Converter mensagens para o padrão NGSI
  - Realiza o envio para o Orion Context Broker para que fiquem disponíveis

# Projeto Lamp
> O projeto tem como principal objetivo explorar e testar os componetes do Fiware.
> O código foi desenvolvido para ser aplicado no DOIT ESP32 DEVKIT V1, o qual possibilita a conexão ao Fiware, através do protocolo 

O programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT, possibilitando, assim, identificar se o led está ligado ou desligado!
> Realize os procedimentos utilizando a collection do Postman.

# O que cada componente irá realizar dentro do projeto 
### IoT Agent
- Apresenta a versão do IoT Agent MQTT
- Verifica se já existe um FIWARE Service cadastrado

#### Criação do FIWARE Service:
- Criação do FIWARE Service (Realizar esse procedimento uma única vez)

#### Criação e registro do dispositivo:
- Provisionamento do dispositivo no IoT Agent MQTT
- Registro do dispositivo virtual no Orion Context Broker

#### Operação do dispostivo:
- Envia comandos on/off para o dispositivo
- Busca no Orion o valor de luminosidade enviado pelo dispositivo
- Busca no Orion o status do LED presente no dispositivo

#### Listar e deletar dispositivos:
- Lista os dispositivos presentes no FIWARE Descomplicado
- Deleta o dispositivo 

### Orion Context Broker 
- Apresenta a versão e diversas informações sobre o Orion Context Broker
- Apresenta os dispositivos cadastrados no FIWARE Descomplicado

### STH-Comet
- Apresenta a versão do STH-Comet
- Realiza a ativação do armazenamento temporal
- Apresenta os valores históricos de acordo com os parâmetros estabelecidos

# Diagrama do projeto
<img width="655" height="524" alt="image" src="https://github.com/user-attachments/assets/3fc27d38-4d83-469e-a0da-3967a4c24716" />

# Código do projeto
<a href = "esp32-codigo.ino">Código para o ESP32 DEVKIT V1<a/>

# Vídeo de execução e funcionamento
https://youtu.be/_IZFtQ9NFyo

# Equipe:
Raphael Aaron - 564067
Felipe Catto - 562106
Kimberly Kristina - 564080
Laura Dantas - 564064


