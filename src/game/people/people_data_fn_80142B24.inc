    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    clrlwi. r0, r5, 16
    stw r31, 0x1c(r1)
    mr r31, r7
    stw r30, 0x18(r1)
    mr r30, r6
    stw r29, 0x14(r1)
    mr r29, r5
    stw r28, 0x10(r1)
    mr r28, r3
    beq @L_80142CD4
    cmplwi r0, 0x23
    blt @L_80142B64
    b @L_80142CD4
    @L_80142B64:
    cmplwi r0, 0xb
    bge @L_80142B80
    mr r3, r4
    bl fn_801440A0
    mr. r28, r3
    bne @L_80142BA4
    b @L_80142CD4
    @L_80142B80:
    cmplwi r0, 0x18
    bge @L_80142B9C
    mr r3, r4
    bl fn_80143DA0
    mr. r28, r3
    bne @L_80142BA4
    b @L_80142CD4
    @L_80142B9C:
    cmplwi r28, 0x0
    beq @L_80142CD4
    @L_80142BA4:
    clrlwi r0, r29, 16
    cmplwi r0, 0x21
    bgt @L_80142CD4
    lis r3, jumptable_80367D60@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80367D60@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r28
    mr r4, r31
    bl fn_80143C40
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 24
    bl fn_80143C30
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_80143C20
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 24
    bl fn_80143C10
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 24
    bl fn_80143C00
    b @L_80142CD4
    mr r3, r28
    mr r4, r31
    bl fn_80143BF0
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_80143BE0
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_80143BD0
    b @L_80142CD4
    mr r3, r28
    mr r4, r30
    extsb r5, r31
    bl fn_80143BB0
    b @L_80142CD4
    mr r3, r28
    mr r4, r31
    bl fn_80143BA0
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_80143B90
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_80143B80
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_80143B70
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_8020A328
    b @L_80142CD4
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_8020A318
    b @L_80142CD4
    mr r3, r28
    mr r4, r31
    bl fn_8020A308
    b @L_80142CD4
    mr r3, r28
    mr r4, r31
    bl fn_8020A2F8
    @L_80142CD4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
