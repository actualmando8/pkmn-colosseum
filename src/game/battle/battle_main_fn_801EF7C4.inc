    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x34(r1)
    li r6, 0x0
    stmw r24, 0x10(r1)
    mr r24, r3
    li r3, 0x0
    bl fn_801F54A4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    li r28, 0x0
    b @L_801EF8D4
@L_801EF824:
    mr r4, r28
    li r3, 0x0
    bl fn_801F47B4
    mr. r31, r3
    beq @L_801EF8D0
    li r30, 0x0
    b @L_801EF8C4
@L_801EF840:
    mr r3, r31
    mr r4, r30
    bl fn_801F7258
    mr. r25, r3
    beq @L_801EF8C0
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801EF8C0
    mr r4, r24
    bl fn_801DA4E8
    li r29, 0x0
    b @L_801EF8B4
@L_801EF87C:
    mr r3, r25
    mr r4, r29
    bl fn_801F981C
    cmplwi r3, 0x0
    beq @L_801EF8B0
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_801EF8B0
    mr r4, r24
    bl fn_801DA4E8
@L_801EF8B0:
    addi r29, r29, 0x1
@L_801EF8B4:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r26
    blt @L_801EF87C
@L_801EF8C0:
    addi r30, r30, 0x1
@L_801EF8C4:
    rlwinm r0, r30, 0, 16, 31
    cmplw r0, r27
    blt @L_801EF840
@L_801EF8D0:
    addi r28, r28, 0x1
@L_801EF8D4:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_801EF824
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
