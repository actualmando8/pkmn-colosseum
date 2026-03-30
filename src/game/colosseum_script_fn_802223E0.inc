    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r5, 0x0
    stw r0, 0x14(r1)
    li r6, 0x0
    li r7, 0x0
    lwz r4, lbl_8047B610(r13)
    lbz r4, 0x1(r4)
    bl fn_801F349C
    cmplwi r3, 0x0
    beq @L_8022241C
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x2(r3)
    b @L_80222424
@L_8022241C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
@L_80222424:
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
