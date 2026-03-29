    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047A4A8(r13)
    mr r3, r4
    cmpwi r0, 0x0
    ble @L_80039628
    li r4, 0x1
    b @L_8003962C
    @L_80039628:
    li r4, 0x0
    @L_8003962C:
    bl fn_80109220
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
