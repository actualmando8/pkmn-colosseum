    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_8047A468(r13)
    cmplwi r0, 0x1
    beq @L_80036614
    bl fn_80165F40
    b @L_80036604
    @L_80036600:
    bl fn_800F0308
    @L_80036604:
    bl fn_801E1874
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80036600
    @L_80036614:
    li r3, 0x39a
    bl fn_800FF58C
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    li r0, 0x1
    stb r0, lbl_8047A468(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
