    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r4
    mr r30, r5
    mr r31, r3
    bne @L_80134BF4
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    mr r31, r3
    @L_80134BF4:
    extsb r0, r30
    cmpwi r0, -0x1
    blt @L_80134C08
    cmpwi r0, 0x3
    blt @L_80134C10
    @L_80134C08:
    li r3, 0x0
    b @L_80134DFC
    @L_80134C10:
    cmplwi r29, 0x0
    bne @L_80134C20
    li r3, 0x0
    b @L_80134DFC
    @L_80134C20:
    cmpwi r0, -0x1
    bne @L_80134CE0
    mr r27, r31
    li r30, 0x0
    b @L_80134CC0
    @L_80134C34:
    li r28, 0x0
    @L_80134C38:
    extsb. r0, r30
    extsb r3, r28
    blt @L_80134C50
    extsb r0, r30
    cmpwi r0, 0x3
    blt @L_80134C58
    @L_80134C50:
    li r3, 0x0
    b @L_80134C7C
    @L_80134C58:
    extsb. r0, r3
    blt @L_80134C68
    cmpwi r3, 0x1e
    blt @L_80134C70
    @L_80134C68:
    li r3, 0x0
    b @L_80134C7C
    @L_80134C70:
    mulli r3, r3, 0x138
    addi r3, r3, 0x14
    add r3, r27, r3
    @L_80134C7C:
    cmplwi r3, 0x0
    beq @L_80134C90
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_80134C9C
    @L_80134C90:
    addi r28, r28, 0x1
    cmpwi r28, 0x1e
    blt @L_80134C38
    @L_80134C9C:
    cmpwi r28, 0x1e
    li r3, -0x1
    bge @L_80134CAC
    extsb r3, r28
    @L_80134CAC:
    extsb. r0, r3
    mr r28, r3
    bge @L_80134CCC
    addi r27, r27, 0x24a4
    addi r30, r30, 0x1
    @L_80134CC0:
    extsb r0, r30
    cmpwi r0, 0x3
    blt @L_80134C34
    @L_80134CCC:
    extsb r0, r30
    cmpwi r0, 0x3
    blt @L_80134D74
    li r3, 0x0
    b @L_80134DFC
    @L_80134CE0:
    mulli r0, r0, 0x24a4
    li r28, 0x0
    add r27, r31, r0
    @L_80134CEC:
    extsb. r0, r30
    extsb r3, r28
    blt @L_80134D04
    extsb r0, r30
    cmpwi r0, 0x3
    blt @L_80134D0C
    @L_80134D04:
    li r3, 0x0
    b @L_80134D30
    @L_80134D0C:
    extsb. r0, r3
    blt @L_80134D1C
    cmpwi r3, 0x1e
    blt @L_80134D24
    @L_80134D1C:
    li r3, 0x0
    b @L_80134D30
    @L_80134D24:
    mulli r3, r3, 0x138
    addi r3, r3, 0x14
    add r3, r27, r3
    @L_80134D30:
    cmplwi r3, 0x0
    beq @L_80134D44
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_80134D50
    @L_80134D44:
    addi r28, r28, 0x1
    cmpwi r28, 0x1e
    blt @L_80134CEC
    @L_80134D50:
    cmpwi r28, 0x1e
    li r3, -0x1
    bge @L_80134D60
    extsb r3, r28
    @L_80134D60:
    extsb. r0, r3
    mr r28, r3
    bge @L_80134D74
    li r3, 0x0
    b @L_80134DFC
    @L_80134D74:
    extsb. r0, r30
    blt @L_80134D88
    extsb r3, r30
    cmpwi r3, 0x3
    blt @L_80134D90
    @L_80134D88:
    li r3, 0x0
    b @L_80134DC0
    @L_80134D90:
    extsb. r0, r28
    blt @L_80134DA4
    extsb r0, r28
    cmpwi r0, 0x1e
    blt @L_80134DAC
    @L_80134DA4:
    li r3, 0x0
    b @L_80134DC0
    @L_80134DAC:
    mulli r3, r3, 0x24a4
    mulli r0, r0, 0x138
    add r3, r3, r0
    addi r3, r3, 0x14
    add r3, r31, r3
    @L_80134DC0:
    cmplwi r3, 0x0
    bne @L_80134DD0
    li r3, 0x0
    b @L_80134DFC
    @L_80134DD0:
    li r0, 0x27
    subi r6, r3, 0x4
    subi r5, r29, 0x4
    mtctr r0
    @L_80134DE0:
    lwz r4, 0x4(r5)
    lwzu r0, 0x8(r5)
    stw r4, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_80134DE0
    bl fn_8012086C
    li r3, 0x1
    @L_80134DFC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
