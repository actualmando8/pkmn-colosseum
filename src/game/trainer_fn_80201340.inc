    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r29, r3
    mr r28, r4
    mr r27, r5
    beq @L_802013EC
    clrlwi r7, r27, 16
    li r4, 0x0
    li r5, 0xf7
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    cmplwi r28, 0x0
    mr r30, r3
    beq @L_802013B4
    mr r5, r28
    li r4, 0x4
    bl fn_8020E57C
    cmplwi r3, 0x0
    beq @L_802013EC
    mr r4, r27
    bl fn_801FD09C
    b @L_802013EC
    @L_802013B4:
    li r28, 0x0
    li r31, 0x0
    @L_802013BC:
    add r29, r30, r31
    mr r3, r29
    bl fn_8020E614
    clrlwi. r0, r3, 24
    beq @L_802013DC
    mr r3, r29
    mr r4, r27
    bl fn_801FD09C
    @L_802013DC:
    addi r28, r28, 0x1
    addi r31, r31, 0xc
    cmplwi r28, 0x4
    blt @L_802013BC
    @L_802013EC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
