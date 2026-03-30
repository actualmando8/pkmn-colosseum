    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x10(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047A94C(r13)
    cmpwi r0, 0x1
    beq @L_800AE61C
    lwz r3, lbl_80478A38(r13)
    bl OSRegisterVersion
    li r31, 0x0
    li r0, 0x1000
    stw r31, lbl_8047A930(r13)
    lis r3, fn_800AE4F4@ha
    stw r31, lbl_8047A928(r13)
    addi r3, r3, fn_800AE4F4@l
    stw r0, lbl_8047A948(r13)
    bl fn_800AC910
    li r0, 0x1
    stw r31, lbl_8047A938(r13)
    stw r31, lbl_8047A93C(r13)
    stw r31, lbl_8047A940(r13)
    stw r31, lbl_8047A944(r13)
    stw r0, lbl_8047A94C(r13)
    @L_800AE61C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr
