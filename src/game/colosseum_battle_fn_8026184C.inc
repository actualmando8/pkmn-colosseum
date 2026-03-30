    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r5
    mr r28, r3
    li r5, 0x4c
    bl fn_801FB1C0
    mr. r30, r3
    bne @L_80261884
    li r3, 0x1
    b @L_80261940
@L_80261884:
    bl fn_801DAC78
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80261898
    li r3, 0x1
    b @L_80261940
@L_80261898:
    lbz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_8026191C
    lwz r31, 0x8(r29)
    stw r28, 0x0(r31)
    sth r3, 0x4(r31)
    mr r3, r30
    bl fn_801DA354
    stb r3, 0x6(r31)
    cmplwi r30, 0x0
    lwz r5, 0x4(r29)
    bne @L_802618D0
    li r4, 0x0
    b @L_802618FC
@L_802618D0:
    li r0, 0x4
    li r3, 0x0
    mtctr r0
@L_802618DC:
    add r4, r5, r3
    lwz r0, 0x0(r4)
    cmplw r0, r30
    bne @L_802618F0
    b @L_802618FC
@L_802618F0:
    addi r3, r3, 0x10
    bdnz @L_802618DC
    li r4, 0x0
@L_802618FC:
    stw r4, 0x8(r31)
    mr r3, r30
    bl fn_801DAC3C
    addi r4, r31, 0xc
    bl fn_800E9C6C
    lwz r3, 0x8(r29)
    addi r0, r3, 0x78
    stw r0, 0x8(r29)
@L_8026191C:
    lbz r0, 0x1(r29)
    cmplwi r0, 0x0
    beq @L_80261930
    mr r3, r30
    bl fn_801DB100
@L_80261930:
    lhz r4, 0x2(r29)
    li r3, 0x1
    addi r0, r4, 0x1
    sth r0, 0x2(r29)
@L_80261940:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
