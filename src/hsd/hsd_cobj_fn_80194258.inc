    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047B230(r13)
    cmplwi r0, 0x0
    beq @L_8019427C
    lwz r3, lbl_8047B230(r13)
    b @L_80194284
@L_8019427C:
    lis r3, lbl_8036C678@ha
    addi r3, r3, lbl_8036C678@l
@L_80194284:
    bl fn_80193828
    mr. r31, r3
    bne @L_801942A0
    la r3, -23880(r2)
    li r4, 0x7a4
    la r5, -23872(r2)
    bl __assert
@L_801942A0:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
