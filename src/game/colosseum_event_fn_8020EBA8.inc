    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r5
    bne @L_8020EBCC
    li r3, 0x0
    b @L_8020EC90
@L_8020EBCC:
    clrlwi r31, r4, 16
    li r29, 0x0
    b @L_8020EC80
@L_8020EBD8:
    clrlwi r3, r29, 16
    extsh. r0, r28
    mulli r0, r3, 0x14
    add r30, r27, r0
    bge @L_8020EC28
    cmplwi r30, 0x0
    bne @L_8020EBFC
    li r0, 0x0
    b @L_8020EC18
@L_8020EBFC:
    mr r3, r30
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020EC14
    li r0, 0x0
    b @L_8020EC18
@L_8020EC14:
    li r0, 0x1
@L_8020EC18:
    clrlwi. r0, r0, 24
    bne @L_8020EC7C
    mr r3, r30
    b @L_8020EC90
@L_8020EC28:
    cmplwi r30, 0x0
    bne @L_8020EC38
    li r0, 0x0
    b @L_8020EC54
@L_8020EC38:
    mr r3, r30
    bl fn_801FBF04
    extsh. r0, r3
    bge @L_8020EC50
    li r0, 0x0
    b @L_8020EC54
@L_8020EC50:
    li r0, 0x1
@L_8020EC54:
    clrlwi. r0, r0, 24
    beq @L_8020EC7C
    mr r3, r30
    bl fn_801FBF04
    extsh r3, r3
    extsh r0, r28
    cmpw r0, r3
    bne @L_8020EC7C
    mr r3, r30
    b @L_8020EC90
@L_8020EC7C:
    addi r29, r29, 0x1
@L_8020EC80:
    clrlwi r0, r29, 16
    cmplw r0, r31
    blt @L_8020EBD8
    li r3, 0x0
@L_8020EC90:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
