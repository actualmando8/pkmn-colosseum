    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r14, 0x28(r1)
    mr r15, r3
    mr r16, r4
    addi r0, r15, 0x1
    li r4, 0x2
    stw r0, 0x18(r1)
    mr r3, r0
    bl fn_8008ABE4
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r18, r0, 0x64
    bl OSGetTick
    lis r5, -32709
    lis r4, -32709
    rlwinm r6, r15, 3, 0, 28
    mr r25, r3
    addi r0, r5, 28184
    rlwinm r26, r15, 2, 0, 29
    add r19, r0, r6
    addi r27, r4, 28168
    addi r20, r19, 0x4
    lwz r5, 0xc(r16)
    rlwinm r3, r5, 8, 24, 31
    rlwinm r0, r5, 24, 16, 23
    rlwinm r4, r5, 8, 8, 15
    rlwinm r5, r5, 24, 0, 7
    or r0, r3, r0
    or r0, r4, r0
    or r0, r5, r0
    rlwinm r14, r0, 16, 16, 31
    rlwinm r23, r0, 0, 16, 31
@L_80071B80:
    bl OSGetTick
    subf r4, r25, r3
    mr r3, r15
    xor r0, r4, r18
    cntlzw r0, r0
    slw r0, r4, r0
    rlwinm r17, r0, 1, 31, 31
    bl fn_80073C38
    cmpwi r3, 0x0
    beq @L_80071BB0
    mr r21, r3
    b @L_80071DF0
@L_80071BB0:
    li r0, 0x22
    mr r3, r15
    stw r0, 0x10(r1)
    addi r4, r1, 0x10
    addi r5, r1, 0xa
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80071BD8
    li r21, 0xb
    b @L_80071CAC
@L_80071BD8:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r22, r0, 0x64
    bl OSGetTick
    mr r21, r3
@L_80071C00:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r22
    ble @L_80071C18
    li r3, 0x1
    b @L_80071C98
@L_80071C18:
    mr r3, r15
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80071C34
    li r3, 0x2
    b @L_80071C98
@L_80071C34:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80071C74
    lwz r12, 0x0(r19)
    cmplwi r12, 0x0
    beq @L_80071C60
    mr r3, r15
    lwz r4, 0x0(r20)
    mtctr r12
    bctrl
@L_80071C60:
    lwzx r0, r27, r26
    cmpwi r0, 0x0
    beq @L_80071C00
    li r3, 0x3e8
    b @L_80071C98
@L_80071C74:
    mr r3, r15
    addi r4, r1, 0x14
    addi r5, r1, 0xa
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80071C94
    li r3, 0x3
    b @L_80071C98
@L_80071C94:
    li r3, 0x0
@L_80071C98:
    cmpwi r3, 0x0
    beq @L_80071CA8
    addi r21, r3, 0xb
    b @L_80071CAC
@L_80071CA8:
    li r21, 0x0
@L_80071CAC:
    cmpwi r21, 0x0
    beq @L_80071CB8
    b @L_80071DF0
@L_80071CB8:
    lwz r0, 0x14(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0x22
    beq @L_80071CD0
    li r21, 0xf
    b @L_80071DF0
@L_80071CD0:
    rlwinm r0, r14, 0, 16, 31
    rlwinm r3, r23, 2, 0, 29
    cmplwi r0, 0x0
    addi r24, r3, 0x10
    beq @L_80071CE8
    addi r24, r24, 0x70
@L_80071CE8:
    li r22, 0x0
    lis r3, 0x1062
    mr r31, r16
    addi r28, r3, 0x4dd3
    lis r29, 0x8000
    b @L_80071DD4
@L_80071D00:
    lwz r0, 0x0(r31)
    mr r3, r15
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80071D28
    li r21, 0x10
    b @L_80071DE0
@L_80071D28:
    lwz r0, 0xf8(r29)
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r28, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r21, r0, 0x64
    bl OSGetTick
    mr r30, r3
@L_80071D44:
    bl OSGetTick
    subf r0, r30, r3
    cmplw r0, r21
    ble @L_80071D5C
    li r21, 0x11
    b @L_80071DE0
@L_80071D5C:
    mr r3, r15
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80071D78
    li r21, 0x12
    b @L_80071DE0
@L_80071D78:
    lbz r0, 0x9(r1)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80071DB8
    lwz r12, 0x0(r19)
    cmplwi r12, 0x0
    beq @L_80071DA4
    mr r3, r15
    lwz r4, 0x0(r20)
    mtctr r12
    bctrl
@L_80071DA4:
    lwzx r0, r27, r26
    cmpwi r0, 0x0
    beq @L_80071D44
    li r21, 0x3e8
    b @L_80071DE0
@L_80071DB8:
    lwzx r0, r27, r26
    cmpwi r0, 0x0
    beq @L_80071DCC
    li r21, 0x3e8
    b @L_80071DE0
@L_80071DCC:
    addi r22, r22, 0x4
    addi r31, r31, 0x4
@L_80071DD4:
    cmpw r22, r24
    blt @L_80071D00
    li r21, 0x0
@L_80071DE0:
    cmpwi r21, 0x0
    beq @L_80071DEC
    b @L_80071DF0
@L_80071DEC:
    li r21, 0x0
@L_80071DF0:
    cmpwi r21, 0x1
    bne @L_80071E00
    cmpwi r17, 0x0
    beq @L_80071B80
@L_80071E00:
    cmpwi r21, 0x0
    lwz r3, 0x18(r1)
    beq @L_80071E14
    li r4, 0x1
    b @L_80071E18
@L_80071E14:
    li r4, 0x3
@L_80071E18:
    bl fn_8008ABE4
    mr r3, r21
    lmw r14, 0x28(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
