    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801BE57C
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801BE504
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801BE540
@L_801BE504:
    lwz r0, lbl_8047B378(r13)
    cmplwi r0, 0x0
    beq @L_801BE518
    lwz r3, lbl_8047B378(r13)
    b @L_801BE520
@L_801BE518:
    lis r3, lbl_8036D3F0@ha
    addi r3, r3, lbl_8036D3F0@l
@L_801BE520:
    bl fn_80193828
    mr. r31, r3
    bne @L_801BE55C
    addi r3, r2, -22512
    li r4, 0x884
    addi r5, r2, -22484
    bl fn_80196E10
    b @L_801BE55C
@L_801BE540:
    bl fn_80193828
    mr. r31, r3
    bne @L_801BE55C
    addi r3, r2, -22512
    li r4, 0x1ed
    addi r5, r2, -22416
    bl fn_80196E10
@L_801BE55C:
    lwz r5, 0x0(r31)
    mr r3, r31
    mr r4, r30
    lwz r12, 0x40(r5)
    mtctr r12
    bctrl
    mr r3, r31
    b @L_801BE580
@L_801BE57C:
    li r3, 0x0
@L_801BE580:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

