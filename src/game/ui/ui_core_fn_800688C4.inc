    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r5
    mr r30, r6
    li r31, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80068918
    bge @L_80068918
    cmpwi r3, 0x0
    bge @L_8006890C
    b @L_80068918
@L_8006890C:
    cmpwi r29, 0x2
    blt @L_80068918
    li r31, 0x0
@L_80068918:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    beq @L_800689DC
    mr r3, r29
    bl fn_8025D914
    bl fn_8012AC54
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80068948
    li r3, 0x1
    bl fn_800FA280
    mr r31, r3
@L_80068948:
    mr r3, r29
    bl fn_8025DA18
    clrlwi r4, r3, 16
    li r3, 0x34
    addi r4, r4, 0x1
    bl fn_80132A38
    mr r4, r31
    li r3, 0x37
    bl fn_80132A38
    bl fn_8025D9CC
    cmpwi r3, 0x4
    bne @L_80068998
    lbz r5, 0x8b(r28)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x30dc
    bl fn_800FB680
    b @L_800689DC
@L_80068998:
    cmpwi r30, 0x2
    bne @L_800689C0
    lbz r5, 0x8b(r28)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x30e6
    bl fn_800FB680
    b @L_800689DC
@L_800689C0:
    lbz r5, 0x8b(r28)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x30dc
    bl fn_800FB680
@L_800689DC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
