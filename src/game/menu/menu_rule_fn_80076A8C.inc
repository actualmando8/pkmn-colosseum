    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r30, r3
    mr r31, r4
    cmpwi r6, 0x2
    beq @L_80076CBC
    bge @L_80076AC0
    cmpwi r6, 0x0
    beq @L_80076ACC
    bge @L_80076ADC
    b @L_80076F14
@L_80076AC0:
    cmpwi r6, 0x4
    bge @L_80076F14
    b @L_80076EB8
@L_80076ACC:
    mr r4, r5
    mr r5, r6
    bl fn_80076F2C
    b @L_80076F18
@L_80076ADC:
    lbz r0, 0xc(r5)
    cmplwi r0, 0x0
    beq @L_80076AF0
    li r3, 0x1
    b @L_80076F18
@L_80076AF0:
    cntlzw r0, r31
    li r29, 0x0
    rlwinm r28, r0, 27, 5, 31
    cmpwi r28, 0x0
    bne @L_80076B20
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076B24
@L_80076B20:
    li r29, 0x1
@L_80076B24:
    cmpwi r29, 0x0
    bne @L_80076BA8
    cmpwi r28, 0x0
    li r28, 0x0
    bne @L_80076B54
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076B58
@L_80076B54:
    li r28, 0x1
@L_80076B58:
    cmpwi r28, 0x0
    beq @L_80076B68
    li r0, 0x0
    b @L_80076B9C
@L_80076B68:
    mr r3, r31
    li r28, 0x0
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076B94
    mr r3, r31
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076B98
@L_80076B94:
    li r28, 0x1
@L_80076B98:
    rlwinm r0, r28, 0, 24, 31
@L_80076B9C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80076BB0
@L_80076BA8:
    li r3, 0x1
    b @L_80076F18
@L_80076BB0:
    li r27, 0x0
@L_80076BB4:
    mr r3, r30
    rlwinm r4, r27, 0, 16, 31
    bl fn_8012AC08
    mr r26, r3
    cmplw r26, r31
    beq @L_80076CA8
    cntlzw r0, r26
    li r29, 0x0
    rlwinm r28, r0, 27, 5, 31
    cmpwi r28, 0x0
    bne @L_80076BF8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076BFC
@L_80076BF8:
    li r29, 0x1
@L_80076BFC:
    cmpwi r29, 0x0
    bne @L_80076CA8
    cmpwi r28, 0x0
    li r28, 0x0
    bne @L_80076C2C
    mr r3, r26
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076C30
@L_80076C2C:
    li r28, 0x1
@L_80076C30:
    cmpwi r28, 0x0
    beq @L_80076C40
    li r0, 0x0
    b @L_80076C74
@L_80076C40:
    mr r3, r26
    li r28, 0x0
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076C6C
    mr r3, r26
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076C70
@L_80076C6C:
    li r28, 0x1
@L_80076C70:
    rlwinm r0, r28, 0, 24, 31
@L_80076C74:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076CA8
    mr r3, r31
    bl fn_8011F5C8
    rlwinm r28, r3, 0, 16, 31
    mr r3, r26
    bl fn_8011F5C8
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r28
    bne @L_80076CA8
    li r3, 0x0
    b @L_80076F18
@L_80076CA8:
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_80076BB4
    li r3, 0x1
    b @L_80076F18
@L_80076CBC:
    lbz r0, 0xd(r5)
    cmplwi r0, 0x0
    beq @L_80076CD0
    li r3, 0x1
    b @L_80076F18
@L_80076CD0:
    cntlzw r0, r31
    li r29, 0x0
    rlwinm r28, r0, 27, 5, 31
    cmpwi r28, 0x0
    bne @L_80076D00
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076D04
@L_80076D00:
    li r29, 0x1
@L_80076D04:
    cmpwi r29, 0x0
    bne @L_80076D88
    cmpwi r28, 0x0
    li r28, 0x0
    bne @L_80076D34
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076D38
@L_80076D34:
    li r28, 0x1
@L_80076D38:
    cmpwi r28, 0x0
    beq @L_80076D48
    li r0, 0x0
    b @L_80076D7C
@L_80076D48:
    mr r3, r31
    li r28, 0x0
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076D74
    mr r3, r31
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076D78
@L_80076D74:
    li r28, 0x1
@L_80076D78:
    rlwinm r0, r28, 0, 24, 31
@L_80076D7C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80076D90
@L_80076D88:
    li r3, 0x1
    b @L_80076F18
@L_80076D90:
    mr r3, r31
    bl fn_8011F1A0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80076DAC
    li r3, 0x1
    b @L_80076F18
@L_80076DAC:
    li r27, 0x0
@L_80076DB0:
    mr r3, r30
    rlwinm r4, r27, 0, 16, 31
    bl fn_8012AC08
    mr r26, r3
    cmplw r26, r31
    beq @L_80076EA4
    cntlzw r0, r26
    li r28, 0x0
    rlwinm r29, r0, 27, 5, 31
    cmpwi r29, 0x0
    bne @L_80076DF4
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076DF8
@L_80076DF4:
    li r28, 0x1
@L_80076DF8:
    cmpwi r28, 0x0
    bne @L_80076EA4
    cmpwi r29, 0x0
    li r29, 0x0
    bne @L_80076E28
    mr r3, r26
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076E2C
@L_80076E28:
    li r29, 0x1
@L_80076E2C:
    cmpwi r29, 0x0
    beq @L_80076E3C
    li r0, 0x0
    b @L_80076E70
@L_80076E3C:
    mr r3, r26
    li r29, 0x0
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076E68
    mr r3, r26
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076E6C
@L_80076E68:
    li r29, 0x1
@L_80076E6C:
    rlwinm r0, r29, 0, 24, 31
@L_80076E70:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076EA4
    mr r3, r31
    bl fn_8011F1A0
    rlwinm r29, r3, 0, 16, 31
    mr r3, r26
    bl fn_8011F1A0
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r29
    bne @L_80076EA4
    li r3, 0x0
    b @L_80076F18
@L_80076EA4:
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_80076DB0
    li r3, 0x1
    b @L_80076F18
@L_80076EB8:
    mr r4, r5
    li r29, 0x0
    li r5, 0x3
    bl fn_80076F2C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076F08
    cmplwi r31, 0x0
    li r30, 0x0
    beq @L_80076EFC
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076F00
@L_80076EFC:
    li r30, 0x1
@L_80076F00:
    cmpwi r30, 0x0
    bne @L_80076F0C
@L_80076F08:
    li r29, 0x1
@L_80076F0C:
    rlwinm r3, r29, 0, 24, 31
    b @L_80076F18
@L_80076F14:
    li r3, 0x0
@L_80076F18:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
