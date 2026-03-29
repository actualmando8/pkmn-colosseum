    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    cmpwi r30, 0x1
    bne @L_80018FE0
    stw r5, lbl_8047A2F4(r13)
    mr r3, r5
    bl fn_801FCEAC
    stw r3, lbl_8047A2F8(r13)
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    stw r0, lbl_8047A2EC(r13)
    blt @L_80018FD4
    cmpwi r0, 0x5
    blt @L_80018FF4
    @L_80018FD4:
    li r0, 0x0
    stw r0, lbl_8047A2EC(r13)
    b @L_80018FF4
    @L_80018FE0:
    li r3, 0x0
    li r0, -0x1
    stw r3, lbl_8047A2F4(r13)
    stw r5, lbl_8047A2F8(r13)
    stw r0, lbl_8047A2EC(r13)
    @L_80018FF4:
    li r0, 0x1
    stw r30, lbl_8047A2E0(r13)
    li r3, 0x1
    li r4, 0x0
    stw r0, lbl_8047A2E4(r13)
    bl fn_8019075C
    li r3, 0x38f
    bl fn_800FF730
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    bl fn_800F0308
    lhz r0, lbl_8047A2F0(r13)
    cmplwi r0, 0x0
    bne @L_80019038
    li r3, 0x0
    b @L_8001904C
    @L_80019038:
    cmplwi r31, 0x0
    beq @L_80019048
    lwz r0, lbl_8047A2EC(r13)
    stw r0, 0x0(r31)
    @L_80019048:
    lhz r3, lbl_8047A2F0(r13)
    @L_8001904C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
