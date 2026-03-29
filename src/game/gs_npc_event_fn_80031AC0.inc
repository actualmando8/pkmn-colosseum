    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047A42C(r13)
    cmpwi r0, 0x7
    bge @L_80031AF4
    cmpwi r0, 0x2
    bge @L_80031AE4
    b @L_80031AF4
    @L_80031AE4:
    mr r3, r4
    li r4, 0x1
    bl fn_80109220
    b @L_80031B00
    @L_80031AF4:
    mr r3, r4
    li r4, 0x0
    bl fn_80109220
    @L_80031B00:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
