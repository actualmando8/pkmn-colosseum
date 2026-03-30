    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr. r31, r3
    beq @L_8021DD98
    li r4, 0x1
    bl fn_80209484
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_8021DD98
    li r4, 0x3
    bl fn_801DA36C
@L_8021DD98:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
