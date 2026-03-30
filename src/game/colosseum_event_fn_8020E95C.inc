    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r29, r3
    mr r30, r4
    mr r31, r5
    bne @L_8020E984
    li r3, 0x0
    b @L_8020EB94
@L_8020E984:
    extsh. r0, r31
    bge @L_8020E994
    li r3, 0x0
    b @L_8020EB94
@L_8020E994:
    cmplwi r29, 0x0
    bne @L_8020E9A4
    li r26, 0x0
    b @L_8020EA60
@L_8020E9A4:
    rlwinm r28, r30, 0, 16, 31
    li r27, 0x0
    b @L_8020EA50
@L_8020E9B0:
    rlwinm r3, r27, 0, 16, 31
    extsh. r0, r31
    mulli r0, r3, 0x14
    add r26, r29, r0
    bge @L_8020E9FC
    cmplwi r26, 0x0
    bne @L_8020E9D4
    li r0, 0x0
    b @L_8020E9F0
@L_8020E9D4:
    mr r3, r26
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020E9EC
    li r0, 0x0
    b @L_8020E9F0
@L_8020E9EC:
    li r0, 0x1
@L_8020E9F0:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_8020EA4C
    b @L_8020EA60
@L_8020E9FC:
    cmplwi r26, 0x0
    bne @L_8020EA0C
    li r0, 0x0
    b @L_8020EA28
@L_8020EA0C:
    mr r3, r26
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020EA24
    li r0, 0x0
    b @L_8020EA28
@L_8020EA24:
    li r0, 0x1
@L_8020EA28:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_8020EA4C
    mr r3, r26
    bl fn_801FBF04
    extsh r3, r3
    extsh r0, r31
    cmpw r0, r3
    bne @L_8020EA4C
    b @L_8020EA60
@L_8020EA4C:
    addi r27, r27, 0x1
@L_8020EA50:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r28
    blt @L_8020E9B0
    li r26, 0x0
@L_8020EA60:
    cmplwi r26, 0x0
    beq @L_8020EA70
    li r3, 0x0
    b @L_8020EB94
@L_8020EA70:
    cmplwi r29, 0x0
    bne @L_8020EA80
    li r27, 0x0
    b @L_8020EAE0
@L_8020EA80:
    rlwinm r30, r30, 0, 16, 31
    li r26, 0x0
    b @L_8020EAD0
@L_8020EA8C:
    rlwinm r0, r26, 0, 16, 31
    mulli r0, r0, 0x14
    add. r27, r29, r0
    bne @L_8020EAA4
    li r0, 0x0
    b @L_8020EAC0
@L_8020EAA4:
    mr r3, r27
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020EABC
    li r0, 0x0
    b @L_8020EAC0
@L_8020EABC:
    li r0, 0x1
@L_8020EAC0:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_8020EACC
    b @L_8020EAE0
@L_8020EACC:
    addi r26, r26, 0x1
@L_8020EAD0:
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r30
    blt @L_8020EA8C
    li r27, 0x0
@L_8020EAE0:
    cmplwi r27, 0x0
    bne @L_8020EAF0
    li r3, 0x0
    b @L_8020EB94
@L_8020EAF0:
    beq @L_8020EB90
    extsh. r0, r31
    blt @L_8020EB90
    mr r3, r27
    li r4, -0x1
    bl fn_801FBE18
    li r26, 0x0
    b @L_8020EB24
@L_8020EB10:
    mr r3, r27
    mr r4, r26
    li r5, 0x0
    bl fn_801FBDF4
    addi r26, r26, 0x1
@L_8020EB24:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020EB10
    mr r3, r27
    li r4, 0x0
    bl fn_801FBDE4
    mr r3, r27
    li r4, 0x0
    bl fn_801FBDD4
    mr r3, r27
    li r4, 0x0
    bl fn_801FBDC4
    mr r3, r27
    li r4, 0x0
    bl fn_801FBDB4
    mr r3, r27
    li r4, 0x0
    bl fn_801FBDA4
    mr r3, r27
    li r4, 0x0
    bl fn_801FBD94
    mr r3, r27
    li r4, 0x0
    bl fn_801FBD84
    mr r3, r27
    mr r4, r31
    bl fn_801FBE18
@L_8020EB90:
    li r3, 0x1
@L_8020EB94:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
