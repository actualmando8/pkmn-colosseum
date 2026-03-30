    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_80209F04
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    li r7, -0x1
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_8011BEB4
    li r4, 0x9
    bl fn_8011B950
    li r3, 0x3f
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    bne @L_80209E44
    mr r3, r31
    li r4, 0x3f
    li r5, 0x0
    bl fn_8011B2C0
@L_80209E44:
    mr r3, r31
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0x9
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
@L_80209F04:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
