    nofralloc
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    lwz r31, lbl_8047A8BC(r13)
    stw r3, lbl_8047A8BC(r13)
    beq @L_800ABF8C
    lis r3, fn_800ABEFC@ha
    addi r3, r3, fn_800ABEFC@l
    bl fn_800CFE3C
    b @L_800ABF98
    @L_800ABF8C:
    lis r3, fn_800ABEFC@ha
    addi r3, r3, fn_800ABEFC@l
    bl fn_800CFF08
    @L_800ABF98:
    mr r3, r31
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
