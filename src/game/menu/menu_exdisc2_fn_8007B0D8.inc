    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047A648(r13)
    cmplwi r4, 0x0
    beq @L_8007B104
    lwz r3, lbl_80478980(r13)
    bl fn_8009AAD4
    li r0, 0x0
    stw r0, lbl_8047A648(r13)
    stw r0, lbl_8047A64C(r13)
@L_8007B104:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
