    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r6, r3
    mr r29, r4
    mr r30, r5
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x1
    bne @L_80014160
    lwz r3, lbl_8047A2F4(r13)
    clrlwi r4, r6, 16
    bl fn_801FCE60
    mr r31, r3
    bl fn_80205BE8
    b @L_80014174
    @L_80014160:
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r5, r6, 16
    li r4, 0x3
    bl fn_8012A5B0
    li r31, 0x0
    @L_80014174:
    stw r3, 0x0(r29)
    stw r31, 0x0(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
