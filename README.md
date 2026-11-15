<!-- coderabbit-ai-generated:v1 -->

# Dynamic Assist (experimental)

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:e1bb89478ea7a66b37aa2d4330478b4d5a60155b72ea0816e5702ff05960e534 -->

Prototipo de asistencia de entrada para Geometry Dash 2.2081 en Windows con Geode 5.11.2. No se ha probado dentro del juego. No se puede garantizar que sea indetectable.

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:d1efb20accf7e296523eca28dab82c5b676051e85559dffb989d29cc3fea66b9 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:3a4e1c75c4e958c7bb51c9672c7a615057c139716b2ea3a062018c325e4e6ddb -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:3da4ec642a79be7aaa438ef98590eae0f978bd00b64bb4f1ab15904d7b104516 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:55d2bf2ee232221451c013b0ba059e1919c2bee7e421341e7e568b799d2fe83d -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:49034b9eec9697ca07f4dce717a827fb3064c97b98504c15f4a6783837968703 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:7ec8df5fc7cb621f32880c1e37bc186d6d7db2d25d2c17a78d3e29d338606048 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:245c43172edfc228355c7ca526cbb857c084735490f163c8274d16760c78f402 -->

## Cambios de la versión 0.6.1

- El límite compartido de predicción por fotograma pasa de 4 ms a 0 por defecto en instalaciones nuevas, para permitir más tiempo a cada búsqueda. El límite individual de 12 ms sigue activo y puede causar tirones. Geode puede conservar un valor configurado anteriormente: revisa **Prediction budget per frame (ms)** y cambia 4 a 0 si quieres quitar el límite compartido.
- Los tipos de los objetos de interacción y sus copias privadas se comprueban una vez por objeto, en lugar de repetir esas comprobaciones en cada paso o candidato. Sus campos, posición y activaciones siguen copiándose y reiniciándose en cada candidato.
- El resultado depende de las interacciones presentes en el nivel. No se ha medido la mejora de FPS ni la cantidad de predicciones dentro del juego.

## Cambios de la versión 0.6.0

<!-- /coderabbit-ai-generated:v1 -->

- Nuevo **Prediction budget per frame (ms)**: todos los clics de un fotograma comparten 4 ms de búsqueda por defecto, además del límite individual. Puede completar menos correcciones que el comportamiento anterior. Usa 0 para recuperar el presupuesto independiente por entrada. Es un límite cooperativo: una operación nativa o su limpieza puede sobrepasarlo.
- Las teclas F6/F7 y los ajustes de interfaz se consultan una vez por fotograma renderizado. La entrega y validación de entradas pendientes conserva su frecuencia de física.
- Se evita preparar el jugador oculto cuando otra entrada interrumpe una espera. Las copias privadas registran su pertenencia una sola vez, y los registros por clic solo se emiten con diagnóstico activado.
- Las pruebas pueden ejecutarse sin Geode: `cmake -S . -B build-tests -DDYNAMIC_TESTS_ONLY=ON -DCMAKE_BUILD_TYPE=Release`, `cmake --build build-tests --config Release` y `ctest --test-dir build-tests -C Release --output-on-failure`. Se mantienen las aserciones también en Release.
- El flujo **Validate and build** valida en Linux y Windows y genera el paquete Windows con el SDK indicado en `mod.json`. Falta medir FPS y estabilidad dentro de Geometry Dash. El binario v0.2.2 del repositorio no contiene estos cambios.

## Cambios de la versión 0.5.3

<!-- /coderabbit-ai-generated:v1 -->

Se registra también el tick real del segundo jugador. La asistencia dual exige relojes compatibles; un cambio del modo de controles invalida una entrada pendiente. El diagnóstico incluye `second_tick_dt`.

<!-- /coderabbit-ai-generated:v1 -->

- La búsqueda termina si el jugador muere antes de recibir el clic candidato: esperar todavía más comparte la misma trayectoria hasta esa muerte. <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:474eb8bf787f98d249d36ac3abe27edf4e437f08a8335d0e183483f227330f6d -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:bca3d41d9ee8dfe3fda629678b4b4e0e0577d8f5c60fdfbd9efc9b95faabb992 -->

En dual, esta optimización se limita igualmente a los pasos anteriores al clic previsto.

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->Se conservan el primer retraso seguro y la prioridad de incertidumbre/presupuesto agotado.
- El diagnóstico muestra `skipped_candidates`, el número de candidatos omitidos por esa comprobación. Una prueba sintética reduce 65 candidatos a 5; no representa una medición de FPS dentro del juego.
- Las copias de objetos quedan retenidas por el jugador oculto hasta su destrucción, además de la caché, para mantener válidas las referencias nativas a contactos y dash durante la limpieza.
- El marcado de efectos privados permanece activo durante `addChild/onEnter` y se restaura con un guard incluso al fallar la creación.

Se mantienen los límites de la versión anterior: no hay simulación conjunta de gravedad enlazada ni mundo futuro completo para Move/Toggle/Spawn. Falta confirmar rendimiento y estabilidad dentro de Geometry Dash.

## Cambios de la versión 0.5.2

<!-- /coderabbit-ai-generated:v1 -->

Se amplía el dual con gravedad independiente a controles separados: cada entrada y su retraso conservan el jugador destinatario. El otro jugador se simula manteniendo sus botones actuales, sin recibir el clic ajeno. Una nueva entrada de cualquiera de los dos entrega primero la pendiente para conservar el orden. El diagnóstico incluye el jugador y el modo de controles. Falta verificarlo dentro del juego.

La gravedad enlazada y la simulación futura de Move/Toggle/Spawn siguen pendientes. El diseño actual no tiene dos jugadores ocultos que avancen juntos ni una copia del estado completo de objetos, grupos y comandos futuros. No se habilitan esas rutas mediante la retirada de protecciones.

## Cambios de la versión 0.5.1

<!-- /coderabbit-ai-generated:v1 -->

- Predicción limitada de dual de un botón con gravedad independiente (`Unlink Dual Gravity`). Se comprueban los dos jugadores para el mismo retraso. Gravedad enlazada, controles separados de dos jugadores, dash, orbes en contacto y futuras interacciones de portales/orbes en dual siguen sin asistirse. Los candidatos incompletos conservan la entrada original.
- Los triggers exclusivamente de color 29, 30, 105, 744, 899, 900 y 915 se omiten en la simulación física. Move, Toggle, Spawn y demás triggers no se consideran decorativos; sigue pendiente simular su efecto futuro en el nivel.
- Al entrar o salir de dual se entrega primero la entrada pendiente. <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:cb9bd46405d7d2f23d19a7e5164aea76a03b2d1976cd82801a68ff75886e4582 -->

Una entrada del segundo jugador tampoco puede adelantarse a una pendiente. Antes de ejecutar un trigger real mediante `EffectGameObject::triggerObject` también se entrega la entrada pendiente, salvo en los triggers de color conocidos, porque el efecto puede invalidar la trayectoria calculada.

<!-- /coderabbit-ai-generated:v1 --> Se conserva el aislamiento de efectos y temporizadores.
- Esta ampliación cambia el bloqueo total de dual de las versiones anteriores. No hay prueba dentro de Geometry Dash ni garantía de ausencia de cierres. Las limitaciones generales de dual descritas abajo se reducen únicamente al caso independiente indicado aquí.

## Cambios de la versión 0.5.0

<!-- /coderabbit-ai-generated:v1 -->

- **Maximum delay** admite hasta 64 ticks (antes 16). F6 y los controles del nivel alcanzan el nuevo límite.
- **Prediction lookahead** permite simular de 32 a 256 pasos después de cada entrada candidata. El valor inicial sigue siendo 32. Supone que mantienes esa entrada durante la ventana; mirar más lejos no garantiza más correcciones.
- **Prediction time budget** admite hasta 66 ms. Su valor inicial sigue siendo 12 ms; el límite superior puede producir pausas visibles en cada clic. Un margen grande necesita más candidatos y puede agotar el presupuesto.
- El jugador oculto conserva las opciones de física `m_enable22Changes`, `m_enableImpulseFix` y `m_gv0123` del jugador real, que no están en el checkpoint. Se reutiliza entre vehículos mediante la restauración nativa, reduciendo construcciones y destrucciones. La limpieza recorre solo las copias usadas en el candidato actual.
- El diagnóstico incluye el horizonte y el presupuesto usados. Se conservan los bloqueos de dual, plataforma e interacciones desconocidas. No se garantiza precisión perfecta ni se ha probado esta versión dentro de Geometry Dash.

## Cambios de la versión 0.4.1

<!-- /coderabbit-ai-generated:v1 -->

- Corrección del aislamiento de las estelas: los checkpoints usados para predecir desactivan `GhostType` y los efectos visuales. Los temporizadores de estelas creadas para la predicción se identifican durante toda su vida, incluso después de retirar el jugador oculto. Esto aborda una vía compatible con los dos cierres recibidos en `GhostTrailEffect::trailSnapshot`; falta confirmar el resultado dentro del juego.
- Se eliminan callbacks y acciones de los nodos privados al terminar cada candidato, también si muere, encuentra una interacción desconocida o agota el tiempo. El jugador real conserva sus efectos.
- Si cambia la duración del tick de física durante una espera, se entrega inmediatamente la entrada pendiente: ya no se mantiene un retraso calculado con otro reloj.
- Se omite la construcción del jugador/checkpoint cuando todavía no hay un tick válido. El presupuesto incluye ahora la preparación y la limpieza de la simulación.

Se mantienen las limitaciones de dual, plataforma y simulación futura de triggers indicadas abajo. Esta versión no garantiza completar todos los timings.


<!-- /coderabbit-ai-generated:v1 -->

## Instalación

Coloca `dynamicassist.practice.geode` en la carpeta `Geometry Dash/geode/mods` y reinicia el juego. La versión del juego y de Geode debe coincidir con las indicadas arriba.

## Uso

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:e6c194d601f543857de41fed979ec786d01c1acdd3e7e0c4810907afb1a8f097 -->

- Empieza desactivado. Los botones **ON/OFF**, **-** y **+** dentro del nivel activan o desactivan la asistencia y ajustan el margen <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:b776099bb648d6db213736f004ecaba2eace9d858dde084e408538133b1619be -->

de 0 a 64 ticks

<!-- /coderabbit-ai-generated:v1 -->. F6 recorre esos valores; F7 alterna entre 0 y el último margen seleccionado (1 si aún no hay uno). También puedes cambiar el margen en los ajustes del mod de Geode.
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:b2cdadc253c6f1e31fd363f0b3f342d78bb24a792106722c443d09c85b277bf6 -->

- La interfaz muestra los clics corregidos, los ticks realmente retrasados, el último retraso y el motivo del último resultado. Una entrada pendiente interrumpida se cuenta solo por los ticks que llegaron a transcurrir; las entradas descartadas no se cuentan. **Show in-level controls** permite ocultar los controles.
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:e79d96579e974a355d365f16a8354abeb23543a6f24382e9604d68168fdf0b3d -->

- Si aparece `window-zero`, activa la asistencia y aumenta el margen. `time-budget` indica que la simulación agotó el tiempo; puedes aumentar **Prediction time budget**. <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:099104d1848cfafa9b229b33d35764e8bed8915e39b3dda004e27c932054805e -->

`Interaccion no simulada` muestra el ID y el tipo numérico del objeto que interrumpió la predicción

<!-- /coderabbit-ai-generated:v1 -->; si activas **Diagnostic log**, estos datos aparecen como `object_id` y `object_type` en el registro. Comparte ambos números y el porcentaje del nivel al informar de un problema. <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:6515b958e9fc606b12e4466db277514215ada4c97c133ddc87737e9a55299cb2 -->

`ring` señala un contacto que no pudo copiarse o un orbe con efectos de grupo. `dual` y `platformer` señalan modos que este prototipo todavía no puede predecir con confianza.

<!-- /coderabbit-ai-generated:v1 --> `no-physics-tick` indica que todavía no hay un tick válido.

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:f98c5183c90792e8e75091694fb1bad7e7d268779ad6d207277e2cf7f46c4b47 -->

<!-- coderabbit-ai-generated-section:v1:81d26a16339d14f979dc1e595068d1b1ba6ccf68332d1b7dcd64f1af9880b803 -->

- **Prediction time budget** ajusta el tiempo máximo dedicado a cada entrada <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:94ecf95c87b156f2f84fc9d1fe7938e410556010aef3ddbbcfe458ae23293564 -->

entre 1 y 66 ms (12 ms por defecto)

<!-- /coderabbit-ai-generated:v1 -->. Un presupuesto mayor permite evaluar más candidatos, pero puede reducir los FPS y no garantiza que la simulación termine. El impacto depende del equipo y del nivel.

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:df0ef8ff5b1acdff7815a22d8ce23d3407b5dbf01a0a055f42258bcf196e87c8 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:f258328710015f692d8ebf1465a151908e2e73545f7fa67ea48850208fc4e5d0 -->

- Intenta corregir pulsaciones y liberaciones tempranas en cubo, nave, OVNI, bola, wave, robot, araña y swing. Retarda una entrada cuando la trayectoria simulada parece más segura; no puede adelantar una pulsación tardía. La asistencia es experimental y no admite dual ni modo plataforma.
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:0922db1ff39fcb3ad49b8fe07f3ebc0849fa77ba978ccd273fa5418acce39bc7 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:53f00f2a509fa1504a21eca1aa38e31ec4c8429809854321cf5ab71edd2a9086 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:2712945a6783f147cc46d17d18a7aeb157e3c0ccae12646d44df9ee368a798cc -->

- Los sólidos, peligros y peligros animados se pasan al motor de colisiones por tipo, sin una lista de IDs. La versión 0.4.0 añade portales de los ocho vehículos, gravedad y tamaño; portales de velocidad 200, 201, 202, 203 y 1334; orbes amarillos, rosas, azules, verdes, rojos, negros, dash y araña, y pads amarillos, rosas, azules, rojos y de araña. Orbes y pads usan copias privadas y sus colisiones nativas. Las activaciones no se escriben sobre los objetos reales. Se requiere comprobar estas interacciones dentro del juego; una compilación correcta no demuestra precisión de física.

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:12823679ed97b1c88876fc869d71cc4e98738e078578f89d0f0d7060688fd0e9 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:ca05be8e88cdea43c0fddd1f04b29aedaa7d662ddecc896d5a1e363bf661807d -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:1785ef27a800626443ffcf36388afdec9edfd39fa1adb96630aee389b051ed24 -->

- Si la predicción no resulta clara o agota su presupuesto, se envía la entrada original. Si llega otra entrada mientras hay una pendiente, esta se entrega inmediatamente antes de la nueva. Los retrasos previstos aparecen en el registro de Geode.
- Una entrada pendiente se entrega también si el tick de física no es válido o si se reduce el margen por debajo de su retraso previsto. F6 y F7 responden mientras la ventana del juego está en primer plano.

<!-- /coderabbit-ai-generated:v1 -->
<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:1458e99181d8ee00db5c592757573eb254e81045f9adbdc82035e2cd0ce64da7 -->

- Para investigar por qué no se corrige un clic, activa **Diagnostic log** en los ajustes del mod y reproduce el problema. El registro local más reciente está en `geode/logs`; busca las líneas `Dynamic Assist diagnostic`. El registro indica también si una entrada pendiente fue reenviada o descartada. Desactiva el diagnóstico cuando termines.

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:96594e1ad332cec64399673a0d15c884ffd6f67e34b92fda5a6ce0113a0120bc -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:2cf99cf24fd2bbaef4886bf41d00ea4e164152c3549ca2d490670be236c4f656 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:f40c5fb8c535bc3bad8e5f47dc00357e63155c68074031703e70560f90de94cc -->

## Optimización 0.7.0 y mod auxiliar

- Dynamic Assist guarda el contexto temporal del nivel una vez por entrada y lo restaura entre candidatos, reutilizando el respaldo. Antes lo volvía a copiar en cada candidato. <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:abe1d1ddc4d54e02d1bff4cb0e85ff75da47004e06cf39aba932789b29addedb -->

La prueba con 65 candidatos reduce las construcciones del respaldo de 65 a 1 y las asignaciones del vector de la prueba de 130 a 2, verificando la misma restauración incluso ante excepciones. La restauración todavía copia datos; la reducción medida es de asignaciones de memoria en esa prueba.

<!-- /coderabbit-ai-generated:v1 --> Esto no significa 65 veces más FPS: la física y la restauración siguen teniendo coste.
- **Dynamic Optimizer 0.1.0** es un segundo mod opcional. Intercepta el registro de timers y lo omite únicamente para nodos privados de una predicción activa. Dynamic Assist ya eliminaba esos timers al terminar cada candidato; se evita insertarlos para eliminarlos inmediatamente después. Mantiene la limpieza de seguridad original.
- Para usarlos juntos, instala `DynamicAssist-v0.7.0.geode` y `DynamicOptimizer-v0.1.0.geode` en `Geometry Dash/geode/mods`, reemplaza la versión anterior de Assist y reinicia el juego. Assist funciona también sin el auxiliar; el auxiliar queda inactivo sin la conexión con Assist.
- Al entrar en un nivel, el log indica `Dynamic Optimizer: connected` o `not installed`. Con Diagnostic log activado, `optimizer=true` confirma la conexión y `skipped_timers` cuenta registros evitados durante esa búsqueda. Puede ser cero en trayectorias que no crean timers.
- No se reducen el horizonte, las colisiones ni los candidatos por este cambio. No se añade soporte para nuevos peligros. Las mejoras de FPS y la integración con otros mods requieren mediciones dentro de GD; no se han medido en este entorno.

## Cobertura 0.6.3

<!-- /coderabbit-ai-generated:v1 -->

La asistencia busca evitar muertes mediante física nativa. La cobertura actual es:

| Situación | Tratamiento |
| --- | --- |
| Sólidos, pinchos, sierras y demás tipos Hazard/AnimatedHazard | Colisiones nativas por tipo, sin lista de IDs de peligros. |
| Pendientes, suelo y techo | Se consulta la física nativa del jugador oculto; requiere verificación dentro del juego. |
| Vehículos, gravedad/tamaño, velocidades, orbes y pads admitidos | Transiciones y física del candidato; grupos personalizados siguen pendientes. |
| Bloques D/H/J/S y contacto 2866 | Copias privadas y `PlayerObject::touchedObject` nativo. |
| Fuerzas 2069/3645 | Copias privadas con los seis parámetros de fuerza; el mapa de IDs consumidos se restaura entre candidatos. |
| Objetos movidos por triggers, teletransporte, grupos custom y lógica futura del nivel | Simulación completa pendiente. El mundo no avanza junto con el jugador oculto; no se garantiza la predicción de estos peligros. |

El diagnóstico añade `predicted_deaths`, `first_death_id` y `first_death_type`: informan de muertes notificadas por el motor durante la búsqueda. La primera muerte puede pertenecer a un candidato descartado, incluso si otro candidato acaba siendo seguro. Un ID `-1` con muertes indica que el motor no entregó un objeto; con cero muertes no hubo notificación. Estos campos se distinguen de `object_id`/`object_type`, que describen una interacción incompatible.

Esta ampliación no cambia los límites de tiempo ni habilita dual o plataforma adicionales. No equivale a compatibilidad universal: requiere pruebas reales de colisiones, fuerzas, dash y pendientes en GD 2.2081.

## Corrección 0.6.2

<!-- /coderabbit-ai-generated:v1 -->

- Los bloques especiales D (1755) y H (1859), antes marcados como incompatibles, se pasan a las colisiones nativas mediante copias privadas. El estado de deslizamiento de wave se restaura entre candidatos.
- El diagnóstico añade `frame_budget_ms` y `deadline_us` para distinguir el límite por entrada del tiempo restante del fotograma.
- Los triggers que cambian el mundo, los orbes personalizados y los portales de teletransporte siguen interrumpiendo la predicción. El registro identifica el primer obstáculo para simular; no demuestra por sí solo que se haya omitido un sólido.
- Fuente: [PlayerObject::touchedObject de GD 2.2081](https://github.com/geode-sdk/bindings/blob/main/bindings/2.2081/inline/PlayerObject.cpp). La precisión de estos contactos requiere comprobarse dentro del juego.

## Limitaciones

<!-- /coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:11de41aa65a875ec4516b7917be272797c74020e1ce8eaf098e5928e4e9135a5 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:21a0002b3666c0616ea719d684b3442af1a14ad4c1e6939a2da4204801993050 -->

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:775bde7f1f5e696a97a062f5173891f8424ce6e9badf953cfb3b844d5074361f -->

La simulación usa un jugador oculto y una ventana corta. Cada candidato se restaura con un checkpoint nativo; <!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:be99b1b723d72b97ed9465769bf12a6d77ff1efdee25281d68489b3875414c48 -->

su contexto de portales y activaciones temporales se restaura al terminar

<!-- /coderabbit-ai-generated:v1 -->, incluso si se agota el presupuesto. El dash ya puede predecirse con una copia de su orbe. Este cambio amplía el comportamiento anterior, que interrumpía la predicción ante orbes, dash y portales de vehículo/velocidad.

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:b5c68e884f09f98f0edc627d496f72a02e3b40485ea355f02b2f3a4c3c4ade84 -->

No se simula el futuro completo del nivel: los triggers, objetos móviles, efectos de otros mods, portales de teletransporte, dual y plataforma siguen requiriendo tratamiento adicional. Los orbes que activan grupos personalizados se consideran inciertos. Las pendientes y fuerzas que ya afectan al jugador usan ahora el estado del checkpoint y la física nativa; esta ampliación sustituye el bloqueo anterior fuera de wave. Los cambios de vehículo usan las funciones nativas del jugador y el contexto del portal, pero no se ha verificado dentro del juego la transición del techo/suelo de la nave ni los ajustes de cámara. Puede fallar o causar resultados inesperados. Esta versión no ofrece todas las funciones del mod privado Dynamic.

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

<!-- /coderabbit-ai-generated:v1 -->

## Compilación

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:144b70d23e249d66e696ca1eb8dd6716e6c6f4cd895dca82cfdff4c9217f5622 -->

Instala Geode SDK 5.11.2 y Geode CLI para Windows, define `GEODE_SDK` con la ruta del SDK y ejecuta `cmake -S . -B build` seguido de `cmake --build build --config Release`.

El flujo de GitHub Actions `Validate and build` ejecuta las pruebas y compila el mod para Windows. Cuando termine correctamente, descarga el artefacto `DynamicAssist-Windows` desde la ejecución. El archivo `DynamicAssist-v0.2.2.geode` incluido en el repositorio es la versión anterior y no contiene estos cambios.

<!-- /coderabbit-ai-generated:v1 -->

## Licencia

<!-- coderabbit-ai-generated:v1 -->

<!-- coderabbit-ai-generated-section:v1:9c7a3e50d3150b7bcb025efbeed0e34df7b1bbd4a00a534d0963f2f65f14b2a1 -->

GPL-3.0-only. La técnica de simulación con un jugador oculto se basa en [GD Mega Overlay](https://github.com/maxnut/GDMegaOverlay), de maxnu y SpaghettDev. Consulta `LICENSE`.

<!-- /coderabbit-ai-generated:v1 -->
