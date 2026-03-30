    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    lis r3, -32698
    li r30, 0x0
    addi r31, r3, 28852
@L_801CAC8C:
    li r0, 0x0
    mr r3, r27
    stw r0, 0x4(r31)
    rlwinm r5, r30, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_801CACF4
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CACF4
    mr r3, r28
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    stw r29, 0x4(r31)
    stw r3, 0x0(r31)
@L_801CACF4:
    addi r31, r31, 0x8
    addi r30, r30, 0x1
    cmpwi r30, 0x6
    blt @L_801CAC8C
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
