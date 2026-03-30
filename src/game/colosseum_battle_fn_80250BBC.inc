    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r5
    li r5, 0x1
    bl fn_80235B04
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    bne @L_80250C08
    li r0, 0xb
    b @L_80250C3C
@L_80250C08:
    cmplwi r0, 0x3
    bne @L_80250C18
    li r0, 0x5
    b @L_80250C3C
@L_80250C18:
    cmplwi r0, 0x1
    bne @L_80250C28
    li r0, 0xa
    b @L_80250C3C
@L_80250C28:
    cmplwi r0, 0x4
    bne @L_80250C38
    li r0, 0xf
    b @L_80250C3C
@L_80250C38:
    li r0, 0x0
@L_80250C3C:
    clrlwi r7, r0, 16
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BBD8
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
