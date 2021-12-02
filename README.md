# Spy Cam Utilizando IoT
Bem vindo ao projeto criado para a disciplina de Objetos Inteligentes Conectados da Universidade Presbiteriana Mackenzie!

<b>Aplicação</b>
A aplicação para o dispositivo é o monitoramento de diferentes ambientes dentro de uma rede. É uma ótima escolha para pais que precisam de liberdade para fazerem suas atividades em casa, mas gostam de estar sempre de olho nas crianças. Com esta aplicação, diversas empresas de segurança oferecem produtos inovadores com o objetivo de detectar criminosos que invadem domicílios. O dispositivo criado nesse artigo poderá ser posicionado em um local determinado de uma casa e transmitir imagens em tempo real via internet. Também será possível movimentar a câmera, que estará fixada em um dispositivo pan/tilt movido por dois pequenos motores (denominados servos), permitindo tanto o movimento horizontal quanto vertical.

<b>Materiais Utilizados</b>
- Placa ESP32 CAM
- Módulo Conversor FTDI USB Serial FT232RL
- Suporte Pan / Tilt Para Arduino para Servo Motor 9G
- 2x Micro Servo Motor 9G SG90 Tower Pro 
- Fonte De Alimentação 3.3v 5v para protoboard
- Fonte DC Chaveada 9V 1A
- Kit de Jumper Cables

<b>Como Utilizar</b>

Segue o diagrama de montagem do ESP32 CAM com a placa conversora USB / Serial e o PC, que utilizará o Arduino IDE para o upload do código.

![image](https://user-images.githubusercontent.com/57954794/144510351-38947b33-9637-482c-88d0-ea1d6e9509e6.png)

Em seguida é necessário configurar a plataforma Arduino IDE para enviar comandos para a ESP32. Para isso, em primeiro lugar, vá em ferramentas / Placa / Gerenciador de Placas e adicione o gerenciador de placas ESP32. Depois disso, selecione a placa AI Thinker ESP32 CAM. Além disso você precisará instalar uma série de bibliotecas que serão utilizadas no código do projeto: ESP32Servo usada para os servos e PubSubClient, usada para o protocolo MQTT. Você pode adicionar essas bibliotecas através do menu Sketch / Incluir biblioteca / Gerenciar Biblio-tecas ou buscar o arquivo ZIP na internet e adicionar manualmente. Carregue o código presente neste repositório para a placa.

A seguir faremos a ligação com a fonte de alimentação e os servos acoplados no suporte pan / tilt. Anote o IP para acessar depois da conexão com os demais componentes.

![image](https://user-images.githubusercontent.com/57954794/144510593-1553589b-6658-4614-bd7c-60427c75bb5f.png)

Conforme ilistrado, a entrada de sinal do servo Tilt deve ser conectada na saída 12 da ESP32 CAM e a entrada de sinal do servo Pan deve ser conectada na saída 2. Com isso a montagem estará completa.

Acesse o IP fornecido no final da ligação com o computador para visualizar o site do dispositivo e controlar a câmera. O formato da página permite o acesso tanto no PC quanto em dispositivos móveis.

Altere o código com as suas credenciais de Wi-Fi, o servidor e tópicos MQTT de sua prefência e em seguida conecte um cliente MQTT para enviar os comandos.

# Obrigado!
