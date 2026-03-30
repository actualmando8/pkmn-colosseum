    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0xf7
    li r6, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    li r4, 0x0
    mr r3, r31
    bl fn_8012640C
    clrlwi. r3, r3, 16
    beq @L_8025CB80
    cmplwi r3, 0x165
    beq @L_8025CB80
    cmplwi r3, 0xffff
    beq @L_8025CB80
    b @L_8025CBD4
@L_8025CB80:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_80201248
    clrlwi. r0, r3, 24
    mr r31, r3
    beq @L_8025CBD0
    bl fn_800E0C54
    clrlwi r5, r3, 16
    clrlwi r4, r31, 24
    divw r0, r5, r4
    addi r3, r1, 0x8
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 24, 1
    lhzx r3, r3, r0
    cmplwi r3, 0x0
    beq @L_8025CBD0
    cmplwi r3, 0x165
    beq @L_8025CBD0
    b @L_8025CBD4
@L_8025CBD0:
    li r3, 0x0
@L_8025CBD4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
