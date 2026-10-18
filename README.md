# Modulo_OUT — módulo de salidas de InduCore

Firmware del módulo de 8 salidas digitales y 2 salidas PWM ("analógicas").
STM32F103C8, se compila con PlatformIO.

Habla el **protocolo del backplane v2** (`src/Core/Inc/induproto.h`,
descripción en [`docs/PROTOCOLO.md`](docs/PROTOCOLO.md)). Funciona con el
firmware de CPU de **inducoreOpenPLC**; no es compatible con versiones
anteriores del CPU.

## Funcionamiento

El módulo es esclavo SPI. Espera a que la CPU lo seleccione (NSS en bajo) y
responde:

- **Detección (`0x48`)**: `ID 0x02` · versión · CRC.
- **Escritura (`0x02`)**: recibe 8 salidas digitales y 2 analógicas de 12 bits
  con CRC-8. Solo si el CRC es correcto aplica las salidas y responde `ACK`;
  si no, responde `NAK` y no cambia nada.

## Seguridad

- **Timeout de comunicación**: si no llega una escritura válida durante
  `SALIDAS_TIMEOUT_MS` (500 ms), apaga todas las salidas. Tiene que ser mayor
  que el período de ciclo del CPU.
- **Watchdog** (IWDG) de ~250 ms. Al reiniciarse, el módulo arranca con todo apagado.
- Al arrancar, PA10/PA11 se ponen en el nivel que apaga las AO antes de
  iniciar el PWM.
- Con el micro **en reset**, las AO quedan encendidas por la lógica invertida
  del optoacoplador. Eso solo se resuelve en la placa.

## Salidas PWM

- 12 bits: TIM1 con `Period = 4094` y prescaler 4 → 3,5 kHz. Un valor de 4095
  es el 100 %.
- `AO_LOGICA_INVERTIDA 1`: el PWM se configura con polaridad invertida para
  compensar el optoacoplador, así 0 = apagada y 4095 = 100 % en la bornera.
  Si se corrige la lógica en el hardware, poner 0.
- Canales: `AN_1` → bornera AO0 (PA10, TIM1 canal 3) y `AN_2` → AO1 (PA11,
  canal 4). Antes estaban cruzados.

## Cambios en esta versión

- Tramas con CRC-8, versión del protocolo y `ACK`/`NAK` en la escritura.
- PWM de 12 bits (antes 8) con polaridad según `AO_LOGICA_INVERTIDA`.
- Canales analógicos corregidos.
- Timeout de comunicación, watchdog y estado seguro al arrancar.
- Se eliminaron la carpeta `EWARM` (IAR) y el test viejo de `test/`, que
  describía el protocolo anterior.

## Pendiente de probar en las placas

1. Que la CPU lo detecte y que `tramasError` y `tramasCrcError` no crezcan.
2. Que 0 = AO apagada y 4095 = 100 % (si queda al revés, `AO_LOGICA_INVERTIDA 0`).
3. Que `AN_1` salga por la bornera AO0.
4. Con salidas encendidas, desconectar o resetear la CPU: en ~0,5 s tienen que
   apagarse todas (`timeoutsComunicacion` sube).
5. Que las AO no se enciendan durante el arranque del módulo.
