    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801F0948
    li r0, 0x0
    b @L_801F0978
    @L_801F0948:
    bl fn_8020D950
    clrlwi. r0, r3, 16
    bne @L_801F095C
    li r0, 0x0
    b @L_801F0978
    @L_801F095C:
    mr r3, r31
    bl fn_8020D920
    cmplwi r3, 0x0
    bne @L_801F0974
    li r0, 0x0
    b @L_801F0978
    @L_801F0974:
    li r0, 0x1
    @L_801F0978:
    clrlwi. r0, r0, 24
    bne @L_801F0988
    li r3, -0x80
    b @L_801F09BC
    @L_801F0988:
    mr r3, r31
    bl fn_8020D920
    cmplwi r3, 0x0
    bne @L_801F09A0
    li r3, -0x80
    b @L_801F09BC
    @L_801F09A0:
    bl fn_8020D82C
    bl fn_8020D7E8
    cmplwi r3, 0x0
    bne @L_801F09B8
    li r3, -0x80
    b @L_801F09BC
    @L_801F09B8:
    bl fn_8020D7CC
    @L_801F09BC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
