# SAMI
Este projeto foi desenvolvido como Trabalho de Conclusão de Curso de graduação em Engenharia Elétrica na FEI.

O SAMI (Sistêma Autônomo de Monitoramento de Incêndio) é um sistema de prevenção e monitoramento de incêndio com a utilização de um drone com uma câmera infravermelho. Os dados são adquiridos pela câmera e enviados ao microcontrolador STM32L475RET6 (de baixo consumo) via comunicação USB, onde são interpretados e enviados ao módulo WiFi ESP8266 via UART. O módulo WiFi, por sua vez, envia os dados organizados à central de controle (servidor) para o monitoramento. 

O projeto consiste no desenvolvimento de uma placa de circuito impresso (esquemático e layout), no desenvolvimento do firmware em C++ para o microcontrolador STM e no software do servidor em C++ no QT.
