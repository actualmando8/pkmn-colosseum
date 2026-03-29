    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    add r5, r3, r4
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r3, lbl_8047AC80(r13)
    mulli r3, r5, 0x18
    stw r4, lbl_8047AC84(r13)
    stw r5, lbl_8047AC88(r13)
    stw r0, lbl_8047AC94(r13)
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AC78(r13)
    beq @L_800FEA64
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, lbl_8047AC7C(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800FEA18
@L_800FEA04:
    lwz r3, lbl_8047AC7C(r13)
    addi r0, r5, 0x8
    addi r5, r5, 0x18
    addi r6, r6, 0x1
    stwx r4, r3, r0
@L_800FEA18:
    lwz r0, lbl_8047AC88(r13)
    cmplw r6, r0
    blt @L_800FEA04
    li r3, 0x2000
    bl fn_800E3534
    sth r3, lbl_8047AC8C(r13)
    clrlwi r3, r3, 16
    bl fn_800E27B0
    mr r4, r3
    lis r3, fn_800FEA74@ha
    stw r4, lbl_8047AC90(r13)
    addi r5, r4, 0x1ffc
    addi r3, r3, fn_800FEA74@l
    li r4, 0x0
    li r6, 0x1ffc
    bl fn_800A263C
    lis r3, fn_800FEBA0@ha
    addi r3, r3, fn_800FEBA0@l
    bl fn_800D30A0
@L_800FEA64:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
