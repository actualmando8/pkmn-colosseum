    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047B378(r13)
    cmplwi r0, 0x0
    beq @L_801BBE00
    lwz r3, lbl_8047B378(r13)
    b @L_801BBE08
@L_801BBE00:
    lis r3, lbl_8036D3F0@ha
    addi r3, r3, lbl_8036D3F0@l
@L_801BBE08:
    bl fn_80193828
    mr. r31, r3
    bne @L_801BBE24
    addi r3, r2, -22512
    li r4, 0x884
    addi r5, r2, -22484
    bl fn_80196E10
@L_801BBE24:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

