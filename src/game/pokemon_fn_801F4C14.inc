    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r4
    mr r26, r5
    mr r27, r6
    mr r29, r3
    mr r30, r7
    li r31, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi. r0, r26, 16
    clrlwi r28, r3, 16
    beq @L_801F4C60
    cmplwi r0, 0x60
    blt @L_801F4C68
    @L_801F4C60:
    li r3, 0x0
    b @L_801F5490
    @L_801F4C68:
    cmplwi r0, 0x8
    bge @L_801F4C88
    mr r3, r25
    bl fn_801F6738
    mr. r29, r3
    bne @L_801F4CAC
    li r3, 0x0
    b @L_801F5490
    @L_801F4C88:
    cmplwi r0, 0x5f
    bge @L_801F4CAC
    cmplwi r29, 0x0
    bne @L_801F4CAC
    bl fn_801F6B48
    mr. r29, r3
    bne @L_801F4CAC
    li r3, 0x0
    b @L_801F5490
    @L_801F4CAC:
    clrlwi r0, r26, 16
    cmplwi r0, 0x5e
    bgt @L_801F548C
    lis r3, jumptable_80375330@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80375330@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r29
    mr r4, r30
    bl fn_801F65F0
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F667C
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r27, 24
    clrlwi r5, r30, 16
    bl fn_801F6658
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 24
    bl fn_801F6648
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6638
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6628
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6618
    b @L_801F548C
    clrlwi r3, r30, 16
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x9
    beq @L_801F4D60
    li r3, 0x0
    b @L_801F4D6C
    @L_801F4D60:
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_8011B444
    @L_801F4D6C:
    clrlwi r0, r3, 24
    mr r31, r3
    cmplwi r0, 0x2
    bne @L_801F548C
    clrlwi r3, r30, 16
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x9
    bne @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    clrlwi r5, r27, 16
    bl fn_8011B2C0
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F68C8
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F68B8
    b @L_801F548C
    cmplwi r30, 0x0
    beq @L_801F4E78
    mr r3, r30
    bl fn_80206780
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F4E38
    mr r4, r30
    li r3, 0xf
    bl fn_80132A38
    mr r3, r30
    bl fn_80207BF4
    clrlwi r3, r3, 16
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x1a
    bl fn_80132A38
    mr r4, r30
    li r3, 0x1f
    bl fn_80132A38
    mr r4, r30
    li r3, 0x21
    bl fn_80132A38
    mr r4, r30
    li r3, 0x20
    bl fn_80132A38
    b @L_801F4EB4
    @L_801F4E38:
    li r3, 0xf
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1a
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1f
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x21
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x20
    li r4, 0x0
    bl fn_80132A38
    b @L_801F4EB4
    @L_801F4E78:
    li r3, 0xf
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1a
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1f
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x21
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x20
    li r4, 0x0
    bl fn_80132A38
    @L_801F4EB4:
    mr r3, r29
    mr r4, r30
    bl fn_801F68A4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xda
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xdb
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xdc
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xdd
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xde
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xdf
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xe0
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xe1
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xe2
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xe3
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    mr r3, r29
    bl fn_801F6A98
    mr r7, r30
    li r4, 0x0
    li r5, 0xe4
    li r6, 0x0
    bl fn_801254B4
    b @L_801F548C
    cmplwi r30, 0x0
    beq @L_801F50D8
    mr r3, r30
    bl fn_80206780
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F5098
    mr r4, r30
    li r3, 0x10
    bl fn_80132A38
    mr r3, r30
    bl fn_80207BF4
    clrlwi r3, r3, 16
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x1b
    bl fn_80132A38
    mr r4, r30
    li r3, 0x42
    bl fn_80132A38
    mr r4, r30
    li r3, 0x44
    bl fn_80132A38
    mr r4, r30
    li r3, 0x43
    bl fn_80132A38
    b @L_801F5114
    @L_801F5098:
    li r3, 0x10
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1b
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x42
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x44
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x43
    li r4, 0x0
    bl fn_80132A38
    b @L_801F5114
    @L_801F50D8:
    li r3, 0x10
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1b
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x42
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x44
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x43
    li r4, 0x0
    bl fn_80132A38
    @L_801F5114:
    mr r3, r29
    mr r4, r30
    bl fn_801F6890
    b @L_801F548C
    mr r3, r29
    mr r7, r30
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    mr r4, r28
    bl fn_801F0134
    mr r0, r3
    mr r3, r29
    clrlwi r7, r0, 16
    li r4, 0x0
    li r5, 0x40
    li r6, 0x0
    bl fn_801F4C14
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F687C
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6868
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F64AC
    b @L_801F548C
    cmplwi r30, 0x0
    beq @L_801F5204
    mr r3, r30
    bl fn_80206780
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F51E8
    mr r4, r30
    li r3, 0x12
    bl fn_80132A38
    mr r3, r30
    bl fn_80207BF4
    clrlwi r3, r3, 16
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x1d
    bl fn_80132A38
    b @L_801F521C
    @L_801F51E8:
    li r3, 0x12
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1d
    li r4, 0x0
    bl fn_80132A38
    b @L_801F521C
    @L_801F5204:
    li r3, 0x12
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1d
    li r4, 0x0
    bl fn_80132A38
    @L_801F521C:
    mr r3, r29
    mr r4, r30
    bl fn_801F647C
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F644C
    b @L_801F548C
    cmplwi r30, 0x0
    beq @L_801F527C
    mr r3, r30
    bl fn_80206780
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F527C
    mr r3, r30
    bl fn_802040E8
    mr r0, r3
    mr r3, r29
    clrlwi r7, r0, 16
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    @L_801F527C:
    mr r3, r29
    mr r4, r30
    bl fn_801F641C
    b @L_801F548C
    mr r3, r30
    bl fn_801F640C
    b @L_801F548C
    cmplwi r30, 0x0
    beq @L_801F5304
    mr r3, r30
    bl fn_80206780
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F52E8
    mr r4, r30
    li r3, 0x1e
    bl fn_80132A38
    mr r3, r30
    bl fn_80207BF4
    clrlwi r3, r3, 16
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x1c
    bl fn_80132A38
    b @L_801F531C
    @L_801F52E8:
    li r3, 0x1e
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1c
    li r4, 0x0
    bl fn_80132A38
    b @L_801F531C
    @L_801F5304:
    li r3, 0x1e
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x1c
    li r4, 0x0
    bl fn_80132A38
    @L_801F531C:
    mr r3, r29
    mr r4, r30
    bl fn_801F6854
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6840
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F682C
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6818
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F6804
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F67F0
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F67DC
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F67C8
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F67B4
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F67A0
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F678C
    b @L_801F548C
    clrlwi r4, r30, 16
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x29
    bl fn_80132A38
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F6778
    b @L_801F548C
    clrlwi r3, r30, 16
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x1c
    bl fn_80132A38
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F6764
    b @L_801F548C
    mr r3, r29
    extsh r4, r30
    bl fn_801F65C0
    b @L_801F548C
    mr r3, r29
    mr r5, r30
    clrlwi r4, r27, 16
    bl fn_801F6560
    b @L_801F548C
    mr r3, r29
    mr r4, r30
    bl fn_801F650C
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F64DC
    b @L_801F548C
    mr r3, r29
    clrlwi r4, r30, 16
    bl fn_801F3984
    b @L_801F548C
    mr r4, r30
    li r3, 0x2f
    bl fn_80132A38
    @L_801F548C:
    clrlwi r3, r31, 24
    @L_801F5490:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
