    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, lbl_8047B288(r13)
    cmpwi r0, 0x0
    beq @L_8019C8D4
    lis r3, lbl_802749E4@ha
    lis r5, lbl_80274A28@ha
    addi r3, r3, lbl_802749E4@l
    li r4, 0x1b6
    addi r5, r5, lbl_80274A28@l
    bl fn_80196E10
    @L_8019C8D4:
    lwz r3, lbl_80478C70(r13)
    mr r4, r31
    bl fn_8009AAD4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
