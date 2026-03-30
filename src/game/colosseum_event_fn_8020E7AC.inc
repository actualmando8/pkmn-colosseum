    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r5
    bne @L_8020E7D0
    li r3, 0x0
    b @L_8020E948
@L_8020E7D0:
    extsh. r0, r28
    bge @L_8020E7E0
    li r3, 0x0
    b @L_8020E948
@L_8020E7E0:
    cmplwi r27, 0x0
    bne @L_8020E7F0
    li r29, 0x0
    b @L_8020E8AC
@L_8020E7F0:
    rlwinm r31, r4, 0, 16, 31
    li r30, 0x0
    b @L_8020E89C
@L_8020E7FC:
    rlwinm r3, r30, 0, 16, 31
    extsh. r0, r28
    mulli r0, r3, 0x14
    add r29, r27, r0
    bge @L_8020E848
    cmplwi r29, 0x0
    bne @L_8020E820
    li r0, 0x0
    b @L_8020E83C
@L_8020E820:
    mr r3, r29
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020E838
    li r0, 0x0
    b @L_8020E83C
@L_8020E838:
    li r0, 0x1
@L_8020E83C:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_8020E898
    b @L_8020E8AC
@L_8020E848:
    cmplwi r29, 0x0
    bne @L_8020E858
    li r0, 0x0
    b @L_8020E874
@L_8020E858:
    mr r3, r29
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020E870
    li r0, 0x0
    b @L_8020E874
@L_8020E870:
    li r0, 0x1
@L_8020E874:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_8020E898
    mr r3, r29
    bl fn_801FBF04
    extsh r3, r3
    extsh r0, r28
    cmpw r0, r3
    bne @L_8020E898
    b @L_8020E8AC
@L_8020E898:
    addi r30, r30, 0x1
@L_8020E89C:
    rlwinm r0, r30, 0, 16, 31
    cmplw r0, r31
    blt @L_8020E7FC
    li r29, 0x0
@L_8020E8AC:
    cmplwi r29, 0x0
    bne @L_8020E8BC
    li r3, 0x0
    b @L_8020E948
@L_8020E8BC:
    mr r3, r29
    li r4, -0x1
    bl fn_801FBE18
    li r30, 0x0
    b @L_8020E8E4
@L_8020E8D0:
    mr r3, r29
    mr r4, r30
    li r5, 0x0
    bl fn_801FBDF4
    addi r30, r30, 0x1
@L_8020E8E4:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020E8D0
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDE4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDD4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDC4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDB4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDA4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBD94
    mr r3, r29
    li r4, 0x0
    bl fn_801FBD84
    li r3, 0x1
@L_8020E948:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
