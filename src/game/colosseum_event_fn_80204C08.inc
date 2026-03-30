    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    mr r30, r3
    li r3, 0x0
    bl fn_801F54A4
    cmplwi r30, 0x0
    bne @L_80204C40
    li r3, 0x0
    b @L_80204CCC
@L_80204C40:
    mr r3, r30
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_80204C64
    li r3, 0x0
    b @L_80204CCC
@L_80204C64:
    bl fn_801F1170
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80204C78
    li r3, 0x0
    b @L_80204CCC
@L_80204C78:
    mr r3, r31
    bl fn_801F0898
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x12
    beq @L_80204C94
    li r3, 0x0
    b @L_80204CCC
@L_80204C94:
    mr r3, r30
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80204CB8
    li r3, 0x0
    b @L_80204CCC
@L_80204CB8:
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r3, r3, 0, 16, 31
@L_80204CCC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
