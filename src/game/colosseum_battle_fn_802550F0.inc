    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    li r3, 0xe
    bl fn_801F025C
    mr. r31, r3
    bne @L_80255120
    li r3, 0x0
    b @L_80255190
@L_80255120:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x19
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r3, 16
    cmplwi r0, 0x2
    blt @L_80255184
    mr r3, r31
    bl fn_802062FC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80255184
    mr r3, r29
    mr r4, r30
    li r5, 0x32
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80255184
    mr r3, r29
    mr r4, r31
    li r5, 0x32
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    beq @L_8025518C
@L_80255184:
    li r3, 0x0
    b @L_80255190
@L_8025518C:
    li r3, 0x1
@L_80255190:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
