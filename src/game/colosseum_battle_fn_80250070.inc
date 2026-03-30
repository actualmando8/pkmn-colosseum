    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x9
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802500A4
    li r3, 0x9
    b @L_802502D8
@L_802500A4:
    mr r3, r30
    mr r4, r31
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802500C8
    li r3, 0xa
    b @L_802502D8
@L_802500C8:
    mr r3, r30
    mr r4, r31
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250100
    mr r3, r30
    mr r4, r31
    bl fn_8023753C
    clrlwi. r0, r3, 24
    bne @L_80250100
    li r3, 0xb
    b @L_802502D8
@L_80250100:
    mr r3, r30
    mr r4, r31
    li r5, 0xe
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250124
    li r3, 0xc
    b @L_802502D8
@L_80250124:
    mr r3, r30
    mr r4, r31
    li r5, 0x17
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250148
    li r3, 0xd
    b @L_802502D8
@L_80250148:
    mr r3, r30
    mr r4, r31
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025016C
    li r3, 0xe
    b @L_802502D8
@L_8025016C:
    mr r3, r30
    mr r4, r31
    li r5, 0x19
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250190
    li r3, 0xf
    b @L_802502D8
@L_80250190:
    mr r3, r30
    mr r4, r31
    li r5, 0x1b
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802501B4
    li r3, 0x10
    b @L_802502D8
@L_802501B4:
    mr r3, r30
    mr r4, r31
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802501D8
    li r3, 0x11
    b @L_802502D8
@L_802501D8:
    mr r3, r30
    mr r4, r31
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802501FC
    li r3, 0x12
    b @L_802502D8
@L_802501FC:
    mr r3, r30
    mr r4, r31
    li r5, 0x26
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250220
    li r3, 0x13
    b @L_802502D8
@L_80250220:
    mr r3, r30
    mr r4, r31
    li r5, 0x27
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250244
    li r3, 0x14
    b @L_802502D8
@L_80250244:
    mr r3, r30
    mr r4, r31
    li r5, 0x28
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250268
    li r3, 0x15
    b @L_802502D8
@L_80250268:
    mr r3, r30
    mr r4, r31
    li r5, 0x29
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025028C
    li r3, 0x16
    b @L_802502D8
@L_8025028C:
    mr r3, r30
    mr r4, r31
    li r5, 0x2a
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802502B0
    li r3, 0x17
    b @L_802502D8
@L_802502B0:
    mr r3, r30
    mr r4, r31
    li r5, 0x30
    bl fn_80236BFC
    clrlwi r3, r3, 24
    li r0, 0x18
    subi r3, r3, 0x1
    subic r3, r3, 0x1
    subfe r3, r3, r3
    and r3, r0, r3
@L_802502D8:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
