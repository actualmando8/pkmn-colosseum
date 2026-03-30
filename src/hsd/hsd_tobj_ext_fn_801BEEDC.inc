    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r4
    beq @L_801BF084
    mr r29, r3
    b @L_801BF07C
@L_801BEF00:
    cmplwi r29, 0x0
    beq @L_801BF078
    lhz r3, 0x4a(r29)
    mr r30, r28
    b @L_801BEF28
@L_801BEF14:
    lwz r0, 0x4(r30)
    cmpw r0, r3
    bne @L_801BEF24
    b @L_801BEF34
@L_801BEF24:
    lwz r30, 0x0(r30)
@L_801BEF28:
    cmplwi r30, 0x0
    bne @L_801BEF14
    li r30, 0x0
@L_801BEF34:
    cmplwi r30, 0x0
    beq @L_801BF078
    lwz r0, 0x64(r29)
    cmplwi r0, 0x0
    beq @L_801BEF50
    lwz r3, 0x64(r29)
    bl fn_801C25E4
@L_801BEF50:
    lwz r3, 0x8(r30)
    bl fn_801C2670
    stw r3, 0x64(r29)
    lwz r0, 0xc(r30)
    stw r0, 0x68(r29)
    lwz r0, 0x6c(r29)
    cmplwi r0, 0x0
    beq @L_801BEFB4
    li r31, 0x0
    b @L_801BEF98
@L_801BEF78:
    lwz r3, 0x6c(r29)
    rlwinm r0, r31, 2, 0, 29
    lwzx r3, r3, r0
    cmplwi r3, 0x0
    beq @L_801BEF94
    li r4, 0x10
    bl fn_80193AF0
@L_801BEF94:
    addi r31, r31, 0x1
@L_801BEF98:
    lwz r3, 0x6c(r29)
    rlwinm r0, r31, 2, 0, 29
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_801BEF78
    lwz r3, 0x6c(r29)
    bl fn_801A6960
@L_801BEFB4:
    lhz r0, 0x16(r30)
    cmplwi r0, 0x0
    beq @L_801BF068
    lhz r3, 0x16(r30)
    addi r0, r3, 0x1
    rlwinm r3, r0, 2, 0, 29
    bl fn_801A6928
    stw r3, 0x6c(r29)
    li r31, 0x0
    b @L_801BF048
@L_801BEFDC:
    lwz r3, 0x10(r30)
    rlwinm r0, r31, 2, 0, 29
    lwzx r27, r3, r0
    cmplwi r27, 0x0
    beq @L_801BF034
    li r3, 0x10
    bl fn_80193B10
    mr. r26, r3
    bne @L_801BF010
    addi r3, r2, -22512
    li r4, 0x8a1
    addi r5, r2, -22492
    bl fn_80196E10
@L_801BF010:
    mr r3, r26
    li r4, 0x0
    li r5, 0x10
    bl memset
    mr r4, r27
    li r5, 0x10
    mr r3, r26
    bl memcpy
    b @L_801BF038
@L_801BF034:
    li r26, 0x0
@L_801BF038:
    lwz r3, 0x6c(r29)
    rlwinm r0, r31, 2, 0, 29
    addi r31, r31, 0x1
    stwx r26, r3, r0
@L_801BF048:
    lhz r0, 0x16(r30)
    cmpw r31, r0
    blt @L_801BEFDC
    lwz r3, 0x6c(r29)
    rlwinm r0, r31, 2, 0, 29
    li r4, 0x0
    stwx r4, r3, r0
    b @L_801BF070
@L_801BF068:
    li r0, 0x0
    stw r0, 0x6c(r29)
@L_801BF070:
    li r0, 0xff
    stb r0, 0x70(r29)
@L_801BF078:
    lwz r29, 0x8(r29)
@L_801BF07C:
    cmplwi r29, 0x0
    bne @L_801BEF00
@L_801BF084:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

