    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r31, r29
@L_800FC3BC:
    lhz r5, 0x0(r31)
    cmplwi r5, 0xffff
    bne @L_800FC3D0
    li r3, 0x0
    b @L_800FC4FC
@L_800FC3D0:
    lwz r4, lbl_80478B08(r13)
    li r7, 0x0
    li r3, 0x0
    lhz r6, 0x4(r4)
    mtctr r6
    cmpwi r6, 0x0
    ble @L_800FC418
@L_800FC3EC:
    lwz r0, 0x24(r4)
    add r30, r0, r3
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    beq @L_800FC40C
    lhz r0, 0x0(r30)
    cmplw r0, r5
    beq @L_800FC418
@L_800FC40C:
    addi r3, r3, 0x8
    addi r7, r7, 0x1
    bdnz @L_800FC3EC
@L_800FC418:
    cmpw r7, r6
    bne @L_800FC4A4
    li r7, 0x0
    li r3, 0x0
    mtctr r6
    cmpwi r6, 0x0
    ble @L_800FC47C
@L_800FC434:
    lwz r0, 0x24(r4)
    add r30, r0, r3
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    bne @L_800FC470
    lwz r4, 0x0(r31)
    addi r5, r31, 0x8
    lwz r3, 0x4(r31)
    li r0, 0x0
    stw r4, 0x0(r30)
    stw r3, 0x4(r30)
    stw r5, 0x4(r30)
    stw r0, 0x8(r5)
    stw r0, 0xc(r5)
    b @L_800FC47C
@L_800FC470:
    addi r3, r3, 0x8
    addi r7, r7, 0x1
    bdnz @L_800FC434
@L_800FC47C:
    lwz r3, lbl_80478B08(r13)
    lhz r0, 0x4(r3)
    cmpw r7, r0
    bne @L_800FC4E4
    lis r3, lbl_8027177C@ha
    lhz r4, 0x0(r31)
    addi r3, r3, lbl_8027177C@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FC4E4
@L_800FC4A4:
    addi r3, r31, 0x8
    lwz r4, 0x4(r30)
@L_800FC4AC:
    cmplw r4, r3
    bne @L_800FC4BC
    li r3, 0x0
    b @L_800FC4FC
@L_800FC4BC:
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800FC4DC
    stw r3, 0x8(r4)
    li r0, 0x0
    stw r0, 0x8(r3)
    stw r4, 0xc(r3)
    b @L_800FC4E4
@L_800FC4DC:
    mr r4, r0
    b @L_800FC4AC
@L_800FC4E4:
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    beq @L_800FC4F8
    add r31, r31, r0
    b @L_800FC3BC
@L_800FC4F8:
    mr r3, r29
@L_800FC4FC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
